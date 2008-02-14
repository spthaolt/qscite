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
#include <string>
#include <iostream>
#include "utils.h"
#include "lexer_utils.h"
using std::string;

#include "mainwindow.h"

MainWindow::MainWindow() {
  openFiles = new std::vector<QsciScintilla *>;
  fileNames = new std::vector<QString>;
  fileNames->push_back(QString(""));
  //QVector<QsciScintilla> tempvector;
  //openFiles = &tempvector;
  tabWidget = new QTabWidget(this);
  setCentralWidget(tabWidget);
  createDocument();
  tabWidget->addTab(curDoc, "Untitled");

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  readSettings();
  connect(curDoc, SIGNAL(textChanged()), this, SLOT(setDocumentModified()));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(curDocChanged()));
  setCurrentFile("");
}

void MainWindow::createDocument() {
  //curDoc->disconnect();
  //disconnect(curDoc, SIGNAL(textChanged()));
  curDoc = new QsciScintilla();
  
  QSettings settings;
  // Default wrap mode to WrapWord
  curDoc->setWrapMode(
  	static_cast<QsciScintilla::WrapMode>(
  		settings.value("wrapMode", QsciScintilla::WrapWord).toInt()
  	)
  );
  // Turn on line numbers by default
  curDoc->setMarginLineNumbers(1, true);
  // set default margin width to 4 characters (will adjust with different files)
  curDoc->setMarginWidth(1, "9999");
  // Don't use tab characters for indents
  curDoc->setIndentationsUseTabs(settings.value("indentUseTabs", false).toBool());
  // Default to using two spaces for each indent
  curDoc->setIndentationWidth(settings.value("indentWidth", 2).toInt());
  // Make backspaces unindent
  curDoc->setBackspaceUnindents(settings.value("backspaceUnindents", true).toBool());
  // Turn on strict brace matching by default
  curDoc->setBraceMatching(
  	static_cast<QsciScintilla::BraceMatch>(
  		settings.value("braceMatchMode", QsciScintilla::StrictBraceMatch).toInt()
  	)
  );
  openFiles->push_back(curDoc);
  fileNames->push_back("");
  tabWidget->addTab(curDoc, "Untitled");
  modified.push_back(false);
  changeTabs(tabWidget->count() - 1);
}

void MainWindow::changeTabs(int index) {
  //std::cout << "attempting to change tabs to index " << index << endl;
  QWidget * nextTab = tabWidget->widget(index);
  tabWidget->setCurrentWidget(nextTab);
  curDocChanged();
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

void MainWindow::newFile() {
  createDocument();
}

bool MainWindow::closeFile() {
  if (tabWidget->count()) {
    if (maybeSave()) {
      QsciScintilla * theEditor = curDoc;
      int curTabIndex = tabWidget->currentIndex();
      tabWidget->removeTab(curTabIndex);
      fileNames->erase(fileNames->begin() + curTabIndex);
      modified.erase(modified.begin() + curTabIndex);
      delete theEditor;
      if (tabWidget->count()) {	// there are tabs left
        changeTabs(curTabIndex > 1 ? curTabIndex - 1 : 0);
      } else {
        // just to make sure...
        curFile = "";
        setWindowTitle(tr("QSciTE"));
      }
      return true;
    }
  }
  
  return false;
}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(this);
  
  if (!fileName.isEmpty()) {
    if ((!tabWidget->count()) || (!curFile.isEmpty()) || modified[tabWidget->currentIndex()]) {
      createDocument();
    }
    
    loadFile(fileName);
  }
}

bool MainWindow::save() {
  if (tabWidget->count()) {
    if (curFile.isEmpty()) {
      return saveAs();
    } else {
      return saveFile(curFile);
    }
  }
  
  // make the compiler happy :)
  return false;
}

bool MainWindow::saveAs() {
  if (tabWidget->count()) {
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()) {
      return false;
    }
  
    return saveFile(fileName);
  }
  
  return false;
}

void MainWindow::fontDialog() {
	if (tabWidget->count()) {
  	QsciLexer * lexer = curDoc->lexer();
  	if (lexer) {
  		lexer->setFont(QFontDialog::getFont( 0, lexer->font(0)));
  		lexer->refreshProperties();
  	} else {
  		curDoc->setFont(QFontDialog::getFont( 0, curDoc->font()));
  	}
	}
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About QSciTE"),
            tr("<b>QSciTE</b> is a clone of SciTE, based on the Scintilla library"
               " and Qt4. It is heavily based on the example code included with"
               " Qscintilla2, and is licensed under the GNU GPL version 2."));
}

void MainWindow::documentWasModified() {
  setWindowModified(modified[tabWidget->currentIndex()]);
  
  QString shownName;
  if (curFile.isEmpty()) {
      shownName = "Untitled";
  } else {
      shownName = strippedName(curFile);
  }
  
  if (modified[tabWidget->currentIndex()]) {
    tabWidget->setTabText(tabWidget->currentIndex(), shownName + "*");
  } else {
    tabWidget->setTabText(tabWidget->currentIndex(), shownName);
  }
}

void MainWindow::setDocumentModified(bool wasModified) {
  modified[tabWidget->currentIndex()] = wasModified;
  documentWasModified();
}

void MainWindow::curDocChanged() {
  curDoc->disconnect(SIGNAL(textChanged()));
  curDoc = (QsciScintilla *) tabWidget->currentWidget();
  connect(curDoc, SIGNAL(textChanged()), this, SLOT(setDocumentModified()));
  curFile = (*fileNames)[tabWidget->currentIndex()];
  setCurrentFile(curFile);
  documentWasModified();
}


void MainWindow::readSettings() {
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings() {
  QSettings settings;
  settings.setValue("pos", pos());
  settings.setValue("size", size());
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
  redoSetMargin();
  QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
  if (newLexer) {
  	newLexer->setParent(curDoc);
  }
  curDoc->setLexer(newLexer);
  setCurrentFile(fileName);
  setDocumentModified(false);
  statusBar()->showMessage(tr("File loaded"), 2000);
}


void MainWindow::redoSetMargin() {
  float numLines = (float)curDoc->lines();
  QString exLength = "99999";
  numLines /= 10000;
  
  while (numLines > 1) {
    exLength += "9";
    numLines /= 10.0;
  }

  curDoc->setMarginWidth(1, exLength);
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
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    setDocumentModified(false);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    (*fileNames)[tabWidget->currentIndex()] = fileName;
    curDoc->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty()) {
        shownName = "Untitled";
    } else {
        shownName = strippedName(curFile);
    }
    tabWidget->setTabText(tabWidget->currentIndex(), shownName);
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("QSciTE")));
}
