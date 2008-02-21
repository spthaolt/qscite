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
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#ifdef QSCITE_DEBUG
#include <iostream>
#endif

#include "mainwindow.h"
#include "utils.h"
#include "lexer_utils.h"
#include "prefs.h"

#ifdef _WIN32

  #include "qterminal.h"
#else
  #include "qterminal_pty.h"
#endif

MainWindow::MainWindow() :
  termWidget(NULL), copyFromTerm(false),
  termInDrawer(QSettings().value("terminalInDrawer", false).toBool())
{
  openFiles = new std::vector<QsciScintilla *>;
  fileNames = new std::vector<QString>;
  fileNames->push_back(QString(""));

  readSettings();
  
  if (!termInDrawer) {
    QSplitter * mainWidget = new QSplitter(this);
    mainWidget->setOrientation(Qt::Vertical);
	  
    tabWidget = new QTabWidget(mainWidget);
    mainWidget->addWidget(tabWidget);
    
    setCentralWidget(mainWidget);
  } else {
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
  }
  
  createDocument();
  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  QToolButton * closeTabButton = new QToolButton(tabWidget);
  closeTabButton->setDefaultAction(closeAct);
  tabWidget->setCornerWidget(closeTabButton);

  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(curDocChanged(int)));
  connect(QApplication::instance(), SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(noticeFocusChange(QWidget *, QWidget *)));
  setWindowTitleForFile("");
}

void MainWindow::createDocument() {
  curDoc = new QsciScintilla();
  applySettingsToDoc(curDoc);
  openFiles->push_back(curDoc);
  fileNames->push_back("");
  modified.push_back(false);
  tabWidget->addTab(curDoc, "Untitled");
  if (tabWidget->count() > 1) {
    changeTabs(tabWidget->count() - 1);
  } else {
    curDocChanged(0);
  }
  connect(curDoc, SIGNAL(copyAvailable(bool)), this, SLOT(updateCopyAvailable(bool)));
}

void MainWindow::changeTabs(int index) {
#ifdef QSCITE_DEBUG
  std::cout << "attempting to change tabs to index " << index << std::endl;
#endif
  tabWidget->setCurrentIndex(index);
}

void MainWindow::closeEvent(QCloseEvent *event) {
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
#ifdef QSCITE_DEBUG
  std::cout << "setDocumentModified(" << wasModified << ')' << std::endl;
 #endif
  int who = tabWidget->currentIndex();
  if (modified[who] != wasModified) {
    modified[who] = wasModified;
	setCurrentTabTitle();
    setWindowModified(wasModified);
  }
}

void MainWindow::curDocChanged(int idx) { 
  curDoc->disconnect(SIGNAL(modificationChanged(bool)));
  curDoc->disconnect(SIGNAL(linesChanged()));
  curDoc = (QsciScintilla *)tabWidget->currentWidget();
  connect(curDoc, SIGNAL(modificationChanged(bool)), this, SLOT(setDocumentModified(bool)));
  connect(curDoc, SIGNAL(linesChanged()), this, SLOT(redoSetMargin()));
  curFile = (*fileNames)[idx];
  setWindowTitleForFile(curFile);
  setWindowModified(modified[idx]);
}

bool MainWindow::maybeSave() {
  if (modified[tabWidget->currentIndex()]) {
    int ret = QMessageBox::warning(this, tr("QSciTE"),
                 tr((strippedName(curFile).toStdString() + " has been modified.\n"
                    "Do you want to save your changes?").c_str()),
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
  QApplication::setOverrideCursor(Qt::WaitCursor);
  curDoc->setText(in.readAll());
  QApplication::restoreOverrideCursor();
  
  curFile = fileName;
  (*fileNames)[tabWidget->currentIndex()] = fileName;

  setWindowTitleForFile(fileName);
  curDoc->setModified(false);
  
  redoSetMargin();
  
  QFont currentFont = curDoc->font();
  QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
  
  if (newLexer != NULL) {
#ifdef QSCITE_DEBUG
    std::cout << "Using lexer " << newLexer->lexer() << std::endl;
#endif
  	newLexer->setParent(curDoc);
    curDoc->setLexer(newLexer);
    setLexerFont(newLexer, currentFont.family(), currentFont.pointSize());
    curDoc->setCaretForegroundColor(QColor(0,0,0));
  }
  
  statusBar()->showMessage(tr("File loaded"), 2000);
}


void MainWindow::redoSetMargin() {
  if (curDoc->marginWidth(1) > 0) {
    double numLines = (double)curDoc->lines();
    QString exLength = "99";
    numLines /= 10;
  
    while (numLines >= 1) {
      exLength += "9";
      numLines /= 10.0;
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
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << curDoc->text();
  QApplication::restoreOverrideCursor();
  statusBar()->showMessage(tr("File saved"), 2000);
  curDoc->setModified(false);
  return true;
}

void MainWindow::setWindowTitleForFile(const QString & fileName) {
  QString shownName;
  
  if (fileName.isEmpty()) {
    shownName = "Untitled";
  } else {
    shownName = strippedName(fileName);
  }
  
  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("QSciTE")));
}

void MainWindow::setCurrentTabTitle() {
	int idx = tabWidget->currentIndex();
	QString displayName = curFile.isEmpty() ? "Untitled" : strippedName(curFile);
	
	if (modified[idx]) {
		displayName += "*";
	}
	
	tabWidget->setTabText(idx, displayName);
}

void MainWindow::updateCopyAvailable(bool yes) {
	if (sender() == QApplication::focusWidget()) {
		cutAct->setEnabled(yes);
		copyAct->setEnabled(yes);
	}
}

void MainWindow::noticeFocusChange(QWidget * prev, QWidget * current) {
	if (termWidget != NULL && current == termWidget) {
		copyFromTerm = true;
	} else if (curDoc != NULL && current == curDoc) {
		copyFromTerm = false;
	}
}
