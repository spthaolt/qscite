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
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <string>
#include <iostream>
#include "utils.h"
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
  curDoc->setMarginLineNumbers(1, true);
  // set default margin width to 4 characters (will adjust with different files)
  curDoc->setMarginWidth(1, "9999");
  openFiles->push_back(curDoc);
  fileNames->push_back("");
  tabWidget->addTab(curDoc, "Untitled");
  modified.push_back(false);
  changeTabs(tabWidget->count() - 1);
}

void MainWindow::changeTabs(int index) {
  std::cout << "attempting to change tabs to index " << index << endl;
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
  
  event->accept();
}

void MainWindow::newFile() {
  createDocument();
}

bool MainWindow::closeFile() {
  if (maybeSave()) {
    QsciScintilla * theEditor = curDoc;
    int curTabIndex = tabWidget->currentIndex();
    tabWidget->removeTab(curTabIndex);
    fileNames->erase(fileNames->begin() + curTabIndex);
    modified.erase(modified.begin() + curTabIndex);
    delete theEditor;
    return true;
  } else {
    return false;
  }
}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(this);
  
  if (!fileName.isEmpty()) {
    createDocument();
    loadFile(fileName);
  }
}

bool MainWindow::save() {
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About Qscite"),
            tr("<b>Qscite</b> is a clone of Scite, based on the Scintilla library"
               " and Qt4. It is heavily based on the example code included with"
               " Qscintilla2, and is licensed under the GNU GPL version 2."));
}

void MainWindow::documentWasModified() {
  //std::cout << "modified flag for " << strippedName(curFile).toStdString() << ": " << curDoc->isModified() << endl;
  setWindowModified(modified[tabWidget->currentIndex()]);
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

void MainWindow::createActions() {
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    
    closeAct = new QAction(QIcon(":/images/close.png"), tr("&Close"), this);
    closeAct->setShortcut(tr("Ctrl+F4"));
    closeAct->setStatusTip(tr("Close the current file"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(closeFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit Qscite"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), curDoc, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), curDoc, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), curDoc, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show Qscite's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(curDoc, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(curDoc, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(closeAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(closeAct);
    fileToolBar->setIconSize(QSize(24, 24));

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->setIconSize(QSize(24, 24));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
    QSettings settings("Trolltech", "Qscite");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings() {
    QSettings settings("Trolltech", "Qscite");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave() {
    if (modified[tabWidget->currentIndex()]) {
        int ret = QMessageBox::warning(this, tr("Qscite"),
                     tr((strippedName(curFile).toStdString() + " has been modified.\n"
                        "Do you want to save your changes?").c_str()),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Qscite"),
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
    setLexerType(fileName);
    setCurrentFile(fileName);
    setDocumentModified(false);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::setLexerType(const QString & fileName) {
    // get the file extension
    string ext = string(tokenize(string(fileName.toAscii().data()), '.').back());

    if (ext == "sh" || ext == "bash") {
        lexer_bash = new QsciLexerBash;
        curDoc->setLexer(lexer_bash);
    } else if (ext == "c" || ext == "cc" || ext == "cxx" || ext == "cpp" || ext == "h") {
        lexer_cpp = new QsciLexerCPP;
        curDoc->setLexer(lexer_cpp);
    } else if (ext == "cs") {
        lexer_csharp = new QsciLexerCSharp;
        curDoc->setLexer(lexer_csharp);
    } else if (ext == "css") {
        lexer_css = new QsciLexerCSS;
        curDoc->setLexer(lexer_css);
    } else if (ext == "html" || ext == "htm" || ext == "php") {
        lexer_html = new QsciLexerHTML;
        curDoc->setLexer(lexer_html);
    } else if (ext == "java") {
        lexer_java = new QsciLexerJava;
        curDoc->setLexer(lexer_java);
    } else if (ext == "pl") {
        lexer_perl = new QsciLexerPerl;
        curDoc->setLexer(lexer_perl);
    } else if (ext == "py") {
        lexer_python = new QsciLexerPython;
        curDoc->setLexer(lexer_python);
    } else if (ext == "rb") {
        lexer_ruby = new QsciLexerRuby;
        curDoc->setLexer(lexer_ruby);
    }
}

void MainWindow::redoSetMargin() {
  float numLines = (float)curDoc->lines();
  QString exLength = "9999";
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
        QMessageBox::warning(this, tr("Qscite"),
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
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Qscite")));
}

QString MainWindow::strippedName(const QString &fullFileName) {
  if (fullFileName.isEmpty()) {
    return "Untitled Document";
  } else {
    return QFileInfo(fullFileName).fileName();
  }
}
