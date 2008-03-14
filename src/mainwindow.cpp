/***************************************************************************
 *   Copyright (C) 2007-2008 by Jared Sutton <jpsutton@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include <QtGui>
#include <QSystemTrayIcon>
#include <QtDebug>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#include "mainwindow.h"
#include "utils.h"
#include "lexer_utils.h"
#include "prefs.h"
#include "textdisplay.h"

#ifdef _WIN32
  #include "qterminal.h"
#else
  #include "qterminal_pty.h"
#endif

MainWindow::MainWindow(QStringList & _argv) :
  argv(_argv),
  termWidget(NULL),
  textSettingsWidget(NULL),
  copyFromTerm(false),
  termInDrawer(QSettings().value("terminalInDrawer", false).toBool()),
  curDocIdx(0)
{
  this->setUnifiedTitleAndToolBarOnMac(true);
  readSettings();
  
  tabWidget = new QTabWidget(this);
  
  // We might need to support a lot of tabs
  tabWidget->setUsesScrollButtons(true);
  tabWidget->setElideMode(Qt::ElideNone);
  
  setCentralWidget(tabWidget);
  this->setWindowIcon(QIcon(":images/appIcon.png"));
  
  createActions();
  createMenus();
  createDocument();
  createToolBars();
  createStatusBar();
  
  if(QSystemTrayIcon::isSystemTrayAvailable() && QSettings().value("trayIcon", true).toBool()) {
    qDebug() << "Creating the system tray icon";
    createTrayIcon();
    trayIcon->setIcon(QIcon(":images/appIcon.png"));
    trayIcon->show();
  }
  

  QToolButton * closeTabButton = new QToolButton(tabWidget);
  closeTabButton->setDefaultAction(closeAct);
  tabWidget->setCornerWidget(closeTabButton);

  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(curDocChanged(int)));
  connect(QApplication::instance(), SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(noticeFocusChange(QWidget *, QWidget *)));
  setWindowTitleForFile("");
  
  for (int i = 0; i < argv.size(); ++i) {
    if (!argv.front().isEmpty()) {
      loadFile(argv.front());
    }
    
    argv.pop_front();
  }
}

void MainWindow::createDocument() {
  QsciScintilla * curDoc = new QsciScintilla();
  curDoc->setUtf8(true);
  applySettingsToDoc(curDoc);

  openFiles.push_back(FileData(curDoc));
  
  tabWidget->addTab(curDoc, "Untitled");
  
  if (tabWidget->count() > 1) {
    changeTabs(tabWidget->count() - 1);
  } else {
    curDocChanged(0);
  }
  
  connect(curDoc, SIGNAL(copyAvailable(bool)), this, SLOT(updateCopyAvailable(bool)));
  connect(curDoc, SIGNAL(modificationChanged(bool)), this, SLOT(setDocumentModified(bool)));
  connect(curDoc, SIGNAL(linesChanged()), this, SLOT(redoSetMargin()));
}

void MainWindow::changeTabs(int index) {
  qDebug() << "attempting to change tabs to index " << index;
  tabWidget->setCurrentIndex(index);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  this->setVisible(true);
  
  if (termWidget != NULL) {
    toggleTerminal();
  }
  
  while(tabWidget->count()) {
    changeTabs(0);
    if (!closeFile()) {
      event->ignore();
      return;
    }
  }
  
  writeSettings();
  event->accept();
}

void MainWindow::setDocumentModified(bool wasModified) {
  qDebug() << "setDocumentModified(" << wasModified << ')';
  setCurrentTabTitle();
  setWindowModified(wasModified);
}

void MainWindow::curDocChanged(int idx) {
  qDebug() << "curDocChanged(" << idx << ')';
  
  curDocIdx = idx;

  if (openFiles.size() > curDocIdx) {
    QsciScintilla * doc = openFiles[curDocIdx].edWidget;
    
    setWindowTitleForFile(openFiles[curDocIdx].baseName);
    setWindowModified(doc->isModified());
    
    setUIForDocumentEolMode();
    
    if (openFiles.size()) {
      codeFoldingAct->setChecked(openFiles[curDocIdx].edWidget->folding());
    }
    
    if (termWidget != NULL && !openFiles[curDocIdx].fullName.isEmpty()) {
      termWidget->changeDir(openFiles[curDocIdx].path);
    }
  }
}

bool MainWindow::maybeSave() {
  if (openFiles[curDocIdx].edWidget->isModified()) {
    int ret = QMessageBox::warning(this, tr("QSciTE"),
                 tr("%1 has been modified.\nDo you want to save your changes?").arg(openFiles[curDocIdx].baseName.isEmpty() ? tr("<Untitled document>") : openFiles[curDocIdx].baseName),
                 QMessageBox::Yes | QMessageBox::Default,
                 QMessageBox::No,
                 QMessageBox::Cancel | QMessageBox::Escape);
    
    if (ret == QMessageBox::Yes) {
      return save();
    } else if (ret == QMessageBox::Cancel) {
      return false;
    }
  }
  
  return true;
}

void MainWindow::loadFile(const QString &fileName) {
  QFile file(fileName);
  
  if (!file.open(QFile::ReadOnly)) {
    QMessageBox::warning(this, tr("QSciTE"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }

  QTextStream in(&file);
  in.setCodec("UTF-8");
  QApplication::setOverrideCursor(Qt::WaitCursor);
  openFiles[curDocIdx].edWidget->setText(in.readAll());
  QApplication::restoreOverrideCursor();
  
  openFiles[curDocIdx].setPathName(fileName);

  setWindowTitleForFile(openFiles[curDocIdx].baseName);
  openFiles[curDocIdx].edWidget->setModified(false);
  
  if (termWidget != NULL) {
    termWidget->changeDir(openFiles[curDocIdx].path);
  }
  
  redoSetMargin();
  
  QsciScintilla * curDoc = openFiles[curDocIdx].edWidget;
  
  QFont currentFont = curDoc->font();
  QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
  
  if (newLexer != NULL) {
    qDebug() << "Using lexer " << newLexer->lexer();
  	newLexer->setParent(curDoc);
    curDoc->setLexer(newLexer);
    setLexerFont(newLexer, currentFont.family(), currentFont.pointSize());
    curDoc->setCaretForegroundColor(QColor(0,0,0));
  }
  
  curDoc->setEolMode(detectEolMode(curDoc));
  
  if (textSettingsWidget != NULL) {
    textSettingsWidget->populate();
  }
    
  statusBar()->showMessage(tr("File loaded"), 2000);
}


void MainWindow::redoSetMargin() {
  QsciScintilla * curDoc = openFiles[curDocIdx].edWidget;
  int numLines = curDoc->lines();
  
  if (numLines < 1000) {
    return;
  }
  
  if (curDoc->marginWidth(1) > 0) {
    QString exLength = "9999";
    numLines /= 1000;
  
    while (numLines >= 1) {
      exLength += "9";
      numLines /= 10;
    }

	  curDoc->setMarginWidth(1, exLength);
  }
}

bool MainWindow::saveFile(const QString &fileName) {
  QFile file(fileName);
  
  if (!file.open(QFile::WriteOnly)) {
    QMessageBox::warning(this, tr("QSciTE"),
                         tr("Cannot write file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return false;
  }

  QTextStream out(&file);
  out.setCodec("UTF-8");
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << openFiles[curDocIdx].edWidget->text();
  QApplication::restoreOverrideCursor();
  statusBar()->showMessage(tr("File saved"), 2000);
  openFiles[curDocIdx].edWidget->setModified(false);

  return true;
}

void MainWindow::setWindowTitleForFile(const QString & fileName) {
  QString shownName;
  
  if (fileName.isEmpty()) {
    shownName = "Untitled";
  } else {
    // strippedName() is gone since we should always be passed a basename
    shownName = fileName;
  }
  
  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("QSciTE")));
}

void MainWindow::setCurrentTabTitle() {
	QString displayName = openFiles[curDocIdx].baseName.isEmpty() ? "Untitled" : openFiles[curDocIdx].baseName;
	
	if (openFiles[curDocIdx].edWidget->isModified()) {
		displayName += "*";
	}
	
	tabWidget->setTabText(curDocIdx, displayName);
}

void MainWindow::updateCopyAvailable(bool yes) {
	if (sender() == QApplication::focusWidget()) {
		cutAct->setEnabled(yes);
		copyAct->setEnabled(yes);
	}
}

void MainWindow::noticeFocusChange(QWidget * prev, QWidget * current) {
  if (prev == current) { }; // make the compiler shut up.
  
	if (termWidget != NULL && current == termWidget) {
		copyFromTerm = true;
	} else if (openFiles.size() > curDocIdx && current == openFiles[curDocIdx].edWidget) {
		copyFromTerm = false;
	}
}

void MainWindow::addRecentFile(const QString & fileName) {
	int maxRecentFiles = QSettings().value("recentFileCount",0).toInt();
	
	recentFiles.push_back(QFileInfo(fileName));
	for (int i = 0; i < recentFiles.size() - 1; ++i) {
		if (recentFiles[i].canonicalFilePath() == recentFiles.back().canonicalFilePath() || !(recentFiles[i].exists() && recentFiles[i].isFile())) {
			recentFiles.removeAt(i--);
		}
		
		while (recentFiles.size() > maxRecentFiles) {
			recentFiles.pop_front();
		}
	}
	
	recentMenu->clear();
	
  for (int i = 0; i < recentFiles.size(); ++i) {
  	recentMenu->addAction( recentFiles[i].fileName() )->setStatusTip( recentFiles[i].filePath() );
  }
  
  recentMenu->menuAction()->setEnabled(!recentFiles.empty());
}

bool MainWindow::eventFilter(QObject * target, QEvent * event) {
	if (target == qApp && event->type() == QEvent::FileOpen) {
		if ((!tabWidget->count()) || (!openFiles[curDocIdx].baseName.isEmpty()) || openFiles[curDocIdx].edWidget->isModified()) {
			createDocument();
		}
		
		QString fileName = dynamic_cast<QFileOpenEvent *>(event)->file();
		loadFile(fileName);
		addRecentFile(fileName);
		setCurrentTabTitle();
		event->accept();
		return true;
	} else {
		return QMainWindow::eventFilter(target, event);
	}
}

void MainWindow::setUIForDocumentEolMode() {
	switch (openFiles[curDocIdx].edWidget->eolMode()) {
		case QsciScintilla::EolWindows:
			lineEndCrLf->setChecked(true);
		break;
		
		case QsciScintilla::EolUnix:
			lineEndLf->setChecked(true);
		break;
		
		case QsciScintilla::EolMac:
			lineEndCr->setChecked(true);
		break;
	}
  showLineEndsAct->setChecked(openFiles[curDocIdx].edWidget->eolVisibility());
}


FileData::FileData(QsciScintilla * doc) : path(QDir::homePath()), edWidget(doc) { ; }
FileData::FileData(const FileData & src) : fullName(src.fullName), baseName(src.baseName), path(src.path), edWidget(src.edWidget) { ; }

void FileData::setPathName(const QString & newPathName) {
	fullName = newPathName;
	QFileInfo info(newPathName);
	baseName = info.fileName();
	path = newPathName.isEmpty() ? "": info.absolutePath();
}
