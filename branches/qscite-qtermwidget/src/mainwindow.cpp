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
#include <QDropEvent>
#include <QUrl>
#include <QList>
#include <QFileInfo>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#include "mainwindow.h"
#include "utils.h"
#include "lexer_utils.h"
#include "prefs.h"
#include "textdisplay.h"
#include "scriptconsole.h"

#include "qtermwidget.h"

MainWindow::MainWindow(QStringList & _argv, Launcher * _launcher) :
  argv(_argv),
  termWidget(NULL),
  textSettingsWidget(NULL),
  copyFromTerm(false),
  termInDrawer(QSettings().value("terminalInDrawer", false).toBool()),
  launcher(_launcher),
  replaceDialog(NULL),
  scriptConsole(NULL)
{
  this->setUnifiedTitleAndToolBarOnMac(true);
  this->setAttribute(Qt::WA_DeleteOnClose);
  readSettings();
  
  tabWidget = new QTabWidget(this);
  
  // We might need to support a lot of tabs
  tabWidget->setUsesScrollButtons(true);
  tabWidget->setElideMode(Qt::ElideNone);
  tabWidget->setTabsClosable(true);
  tabWidget->setMovable(true);
  tabWidget->setFocusPolicy(Qt::ClickFocus);
  connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
  
  setCentralWidget(tabWidget);
  this->setWindowIcon(QIcon(":images/appIcon.png"));
  
  createActions();
  createMenus();
  createDocument();
  createToolBars();
  createStatusBar();  

/*
  QToolButton * closeTabButton = new QToolButton(tabWidget);
  closeTabButton->setDefaultAction(closeAct);
  tabWidget->setCornerWidget(closeTabButton);
*/

  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(curDocChanged(int)));
  connect(QApplication::instance(), SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(noticeFocusChange(QWidget *, QWidget *)));
  setWindowTitleForFile("");

  //accept drag and drop events, handled in dropEvent()
  setAcceptDrops(true);
  
  while (!argv.isEmpty()) {
    if (!argv.front().isEmpty()) {
      setupDocument(argv.front());
    }
    
    argv.pop_front();
  }
  
  //add the window into the script engine.
  QScriptValue window = scriptEngine.newQObject(this);
  scriptEngine.globalObject().setProperty("window", window);
}

void MainWindow::createDocument() {
  QsciteEditor * curDoc = new QsciteEditor(this);
  curDoc->setUtf8(true);
  applySettingsToDoc(curDoc);

  openFiles.insert(curDoc, FileData(curDoc));
  
  tabWidget->addTab(curDoc, "Untitled");
  
  if (tabWidget->count() > 1) {
    changeTabs(curDoc);
  } else {
    curDocChanged(0);
  }
  
  connect(curDoc, SIGNAL(copyAvailable(bool)), this, SLOT(updateCopyAvailable(bool)));
  connect(curDoc, SIGNAL(modificationChanged(bool)), this, SLOT(setDocumentModified(bool)));
  connect(curDoc, SIGNAL(linesChanged()), this, SLOT(redoSetMargin()));
}

void MainWindow::changeTabs(QsciteEditor * edWidget) {
  tabWidget->setCurrentIndex(tabWidget->indexOf(edWidget));
}

void MainWindow::changeTabs(int tabIndex) {
  tabWidget->setCurrentIndex(tabIndex);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  closeWindow() ? event->accept() : event->ignore();
}

bool MainWindow::closeWindow () {
  this->setVisible(true);
  
  if (termWidget != NULL) {
    toggleTerminal();
  }
  
  while(tabWidget->count()) {
    changeTabs(0);
    if (!closeFile()) {
      return false;
    }
  }
  
  writeSettings();
  //emit closed();
  return true;
}

void MainWindow::setDocumentModified(bool wasModified) {
  qDebug() << "setDocumentModified(" << wasModified << ')';
  setCurrentTabTitle();
  setWindowModified(wasModified);
}

void MainWindow::curDocChanged(int idx) {
  if (idx == idx) { } //make compiler shut up

  if (openFiles.size() > 0) {
    QsciteEditor * doc = getCurDoc();
    
    setWindowTitleForFile(getCurFileObj()->baseName);
    setWindowModified(doc->isModified());
    
    setUIForDocumentEolMode();
    
    if (openFiles.size()) {
      codeFoldingAct->setChecked(getCurDoc()->folding());
    }
    
    if (termWidget != NULL && !getCurFileObj()->fullName.isEmpty()) {
      //FIXME: the following line should be re-implemented for qtermwidget
      //termWidget->changeDir(getCurFileObj()->path);
    }

    getCurDoc()->setFocus(Qt::MouseFocusReason);
  }
  
  //have to update the document in the scriptEngine.
  QScriptValue document = scriptEngine.newQObject(getCurDoc());
  scriptEngine.globalObject().setProperty("document", document);
}

bool MainWindow::maybeSave() {
  if (getCurDoc()->isModified()) {
    int ret = QMessageBox::warning(this, tr("QSciTE"),
                 tr("%1 has been modified.\nDo you want to save your changes?").arg(getCurFileObj()->baseName.isEmpty() ? tr("<Untitled document>") : getCurFileObj()->baseName),
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
  getCurDoc()->setText(in.readAll());
  QApplication::restoreOverrideCursor();
  
  getCurFileObj()->setPathName(fileName);

  setWindowTitleForFile(getCurFileObj()->baseName);
  getCurDoc()->setModified(false);
  
  if (termWidget != NULL) {
    //FIXME: re-implement for qtermwidget
    //termWidget->changeDir(getCurFileObj()->path);
  }
  
  redoSetMargin();
  
  QsciteEditor * curDoc = getCurDoc();
  
  QFont currentFont = curDoc->font();
  QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
  
  if (newLexer != NULL) {
/*    
    qDebug() << "Using lexer " << newLexer->lexer();
  	newLexer->setParent(curDoc);
    curDoc->setLexer(newLexer);
    setLexerFont(newLexer, currentFont.family(), currentFont.pointSize());
    curDoc->setCaretForegroundColor(QColor(0,0,0));
*/
    setLexer(newLexer);
  }
  
  curDoc->setEolMode(detectEolMode(curDoc));
  
  if (textSettingsWidget != NULL) {
    textSettingsWidget->populate();
  }
    
  statusBar()->showMessage(tr("File loaded"), 2000);
}


void MainWindow::redoSetMargin() {
  getCurDoc()->setMarginWidth(1, QString::number(getCurDoc()->lines()) + "9");
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
  out << getCurDoc()->text();
  QApplication::restoreOverrideCursor();
  statusBar()->showMessage(tr("File saved"), 2000);
  getCurDoc()->setModified(false);

  return true;
}

void MainWindow::setWindowTitleForFile(const QString & fileName) {
  qDebug() << "Setting window title to " << fileName;
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
  qDebug() << getCurFileObj()->fullName;
	QString displayName = getCurFileObj()->baseName.isEmpty() ? "Untitled" : getCurFileObj()->baseName;
	
	if (getCurDoc()->isModified()) {
		displayName += "*";
	}
	
	tabWidget->setTabText(getCurTabIndex(), displayName);
}

void MainWindow::updateCopyAvailable(bool yes) {
	if (sender() == QApplication::focusWidget()) {
		cutAct->setEnabled(yes);
		copyAct->setEnabled(yes);
	}
}

void MainWindow::setLexer(const QString & lexerName) {
  QsciteEditor * curDoc = getCurDoc();
  QsciLexer * newLexer = NULL;
  newLexer = getLexerByName(lexerName);
  newLexer->setParent(curDoc);
  QFont curFont = curDoc->font();
  setLexerFont(newLexer, curFont.family(), curFont.pointSize());
  curDoc->setLexer(newLexer);
  curDoc->recolor();
}

void MainWindow::setLexer(QsciLexer * lexer) {
  setLexer(QString(lexer->lexer()));
}

void MainWindow::noticeFocusChange(QWidget * prev, QWidget * current) {
  if (prev == current) { }; // make the compiler shut up.
  
	if (termWidget != NULL && current == termWidget) {
		copyFromTerm = true;
	} else if (openFiles.size() > 0 && current == getCurDoc()) {
		copyFromTerm = false;
	}
}

void MainWindow::addRecentFile(const QString & fileName) {
  int maxRecentFiles = QSettings().value("recentFileCount",10).toInt();
	
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
/*  if (event->type() == QEvent::KeyPress) {
    qDebug() << "it's a key press event";
    QKeyEvent *kevent = (QKeyEvent *)event;
  if (kevent->key() == Qt::Key_Tab && kevent->modifiers() & Qt::ShiftModifier) {
    qDebug() << "Shift-tab pressed";
  }
  }	*/  


	if (target == qApp && event->type() == QEvent::FileOpen) {
		QString fileName = dynamic_cast<QFileOpenEvent *>(event)->file();
		setupDocument(fileName);
		event->accept();
		return true;
	} else {
		return QMainWindow::eventFilter(target, event);
	}
}

//load a new document and set up its tab and the associated application metadata
void MainWindow::setupDocument(QString &fileName) {
	//create a new tab, if necessary
	if ((!tabWidget->count()) || (!getCurFileObj()->baseName.isEmpty()) || getCurDoc()->isModified()) {
		createDocument();
	}

	//load the file and set up other metadata
	loadFile(fileName);
	addRecentFile(fileName);
	setCurrentTabTitle();
}

void MainWindow::setUIForDocumentEolMode() {
	switch (getCurDoc()->eolMode()) {
		case QsciteEditor::EolWindows:
			lineEndCrLf->setChecked(true);
		break;
		
		case QsciteEditor::EolUnix:
			lineEndLf->setChecked(true);
		break;
		
		case QsciteEditor::EolMac:
			lineEndCr->setChecked(true);
		break;
	}
	
  showLineEndsAct->setChecked(getCurDoc()->eolVisibility());
}

//triggered when a dragged object enters the window
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	qDebug() << "Drag enter event triggered.\n";
	//only accept the drop if the dropped item has a URL
	if (event->mimeData()->hasFormat("text/uri-list")) {
		qDebug() << event->proposedAction() << "\n";
		event->acceptProposedAction();
	}
}

//triggers when a dragged object is dropped into the window
void MainWindow::dropEvent(QDropEvent *event) {
	QFileInfo fileInfo;
	//get the first URL in the list and open it if it refers to a file
	QString fileName = event->mimeData()->urls().first().toLocalFile();
	fileInfo.setFile(fileName);
	if (fileInfo.isFile()) {
		qDebug() << "Got drop event. Filename is: " << fileName << "\n";
		open(fileName);
	}

	event->acceptProposedAction();
}

FileData::FileData(QsciteEditor * doc) : path(QDir::homePath()), edWidget(doc) { ; }
FileData::FileData(const FileData & src) : fullName(src.fullName), baseName(src.baseName), path(src.path), edWidget(src.edWidget) { ; }

void FileData::setPathName(const QString & newPathName) {
	fullName = newPathName;
  qDebug() << "Full file name is " << newPathName;
	QFileInfo info(newPathName);
	baseName = info.fileName();
	path = newPathName.isEmpty() ? "": info.absolutePath();
}

