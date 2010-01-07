#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include "mainwindow.h"
#include "lexer_utils.h"

void MainWindow::createActions() {
  newAct = new QAction(QIcon(":/images/filenew.png"), tr("&New"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  closeAct = new QAction(QIcon(":/images/fileclose.png"), tr("&Close"), this);
#ifndef Q_WS_MAC
  closeAct->setShortcut(tr("Ctrl+F4"));
#else
  closeAct->setShortcut(tr("Ctrl+W"));
#endif
  closeAct->setStatusTip(tr("Close the current file"));
  connect(closeAct, SIGNAL(triggered()), this, SLOT(closeFile()));

  openAct = new QAction(QIcon(":/images/fileopen.png"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(QIcon(":/images/filesave.png"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct = new QAction(QIcon(":/images/filesaveas.png"), tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  exitAct = new QAction(QIcon(":/images/fileexit.png"), tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit QSciTE"));
  connect(exitAct, SIGNAL(triggered()), launcher, SLOT(quitApplication()));

  undoAct = new QAction(QIcon(":/images/undo.png"), tr("Undo"), this);
  undoAct->setShortcut(tr("Ctrl+Z"));
  undoAct->setStatusTip(tr("Undo the last action performed."));
  connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

  redoAct = new QAction(QIcon(":/images/redo.png"), tr("Redo"), this);
  redoAct->setShortcut(tr("Ctrl+Shift+Z"));
  redoAct->setStatusTip(tr("Redo an action previously undone."));
  connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

  cutAct = new QAction(QIcon(":/images/editcut.png"), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                          "clipboard"));
  connect(cutAct, SIGNAL(triggered()), this, SLOT(editCut()));

  copyAct = new QAction(QIcon(":/images/editcopy.png"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  connect(copyAct, SIGNAL(triggered()), this, SLOT(editCopy()));

  pasteAct = new QAction(QIcon(":/images/editpaste.png"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                            "selection"));
  connect(pasteAct, SIGNAL(triggered()), this, SLOT(editPaste()));

  prefsAct = new QAction(QIcon(":/images/configure.png"), tr("P&references"), this);
  // TODO: set shortcut, tip, etc.
  connect(prefsAct, SIGNAL(triggered()), this, SLOT(globalPrefs()));

  fontAct = new QAction(QIcon(":/images/font.png"), tr("&Font"), this);
  //fontAct->setShortcut(tr(""))
  fontAct->setStatusTip(tr("Set the display font."));

  connect(fontAct, SIGNAL(triggered()), this, SLOT(fontDialog()));

  terminalAct = new QAction(QIcon(":/images/terminal.png"), tr("Terminal"), this);
  connect(terminalAct, SIGNAL(triggered()), this, SLOT(toggleTerminal()));

  textDisplayAct = new QAction(QIcon(":/images/font.png"), tr("Text &Display..."), this);
  connect(textDisplayAct, SIGNAL(triggered()), this, SLOT(textDisplay()));

  aboutAct = new QAction(tr("&About QSciTE"), this);
  aboutAct->setStatusTip(tr("Show QSciTE's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  nextAct = new QAction(QIcon(":/images/nextTab.png"), tr("Forward"), this);
  nextAct->setStatusTip(tr("Change to the next open document"));
  nextAct->setShortcut(tr("Alt+Right"));
  connect(nextAct, SIGNAL(triggered()), this, SLOT(nextDoc()));

  prevAct = new QAction(QIcon(":/images/prevTab.png"), tr("Back"), this);
  prevAct->setStatusTip(tr("Change to the previous open document"));
  prevAct->setShortcut(tr("Alt+Left"));
  connect(prevAct, SIGNAL(triggered()), this, SLOT(prevDoc()));

  cutAct->setEnabled(false);
  copyAct->setEnabled(false);

  convertEndings = new QAction(tr("&Convert Line End Characters"), this);
  connect(convertEndings, SIGNAL(triggered()), this, SLOT(convertEols()));
  lineEndCr = new QAction(tr("&CR (old Macintosh)"), this);
  lineEndCr->setCheckable(true);
  connect(lineEndCr, SIGNAL(triggered()), this, SLOT(setEolCr()));
  lineEndLf = new QAction(tr("&LF (Unix)"), this);
  lineEndLf->setCheckable(true);
  connect(lineEndLf, SIGNAL(triggered()), this, SLOT(setEolLf()));
  lineEndCrLf = new QAction(tr("CR &+ LF (Windows)"), this);
  lineEndCrLf->setCheckable(true);
  connect(lineEndCrLf, SIGNAL(triggered()), this, SLOT(setEolCrLf()));

  lineEnds = new QActionGroup(this);
  lineEnds->addAction(lineEndCr);
  lineEnds->addAction(lineEndLf);
  lineEnds->addAction(lineEndCrLf);
  lineEndLf->setChecked(true);

  showLineEndsAct = new QAction(tr("&Show End of Line"), this);
  showLineEndsAct->setCheckable(true);
  connect(showLineEndsAct, SIGNAL(toggled(bool)), this, SLOT(setEolVisibility(bool)));

  convertIndentAct = new QAction(tr("Convert &Indentation..."), this);
  connect(convertIndentAct, SIGNAL(triggered()), this, SLOT(convertIndentation()));

  codeFoldingAct = new QAction(tr("Use Code Folding"), this);
  codeFoldingAct->setCheckable(true);
  connect(codeFoldingAct, SIGNAL(triggered()), this, SLOT(toggleFolding()));

  findTextAct = new QAction(tr("&Find..."), this);
  findTextAct->setShortcut(tr("Ctrl+F"));
  connect(findTextAct, SIGNAL(triggered()), this, SLOT(showFindDialog()));
  
  replaceTextAct = new QAction(tr("Replace..."), this);
  replaceTextAct->setShortcut(tr("Ctrl+H"));
  connect(replaceTextAct, SIGNAL(triggered()), this, SLOT(showReplaceDialog()));

  newWindowAct = new QAction(QIcon(":/images/newwindow.png"), tr("&New Window"), this);
  connect(newWindowAct, SIGNAL(triggered()), this, SLOT(newWindow()));

  lexers = new QActionGroup(this);
  for (int i = 0; !supportedLexers[i].isEmpty(); ++i) {
    QAction * tmp = new QAction(tr(supportedLexers[i].toStdString().c_str()), this);
    tmp->setCheckable(true);
    connect(tmp, SIGNAL(triggered()), this, SLOT(setLexer()));
    lexers->addAction(tmp);
    //todo: add connections for signal/slot
  }
  
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addSeparator();
  fileMenu->addAction(openAct);
  recentMenu = fileMenu->addMenu(tr("Open &Recent"));
  recentMenu->setIcon(QIcon(":/images/fileopen.png"));
  fileMenu->addAction(closeAct);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(undoAct);
  editMenu->addAction(redoAct);
  editMenu->addSeparator();
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);
  editMenu->addSeparator();
  editMenu->addAction(findTextAct);
  editMenu->addAction(replaceTextAct);
  editMenu->addSeparator();
  editMenu->addAction(prefsAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(textDisplayAct);
  viewMenu->addAction(fontAct);
  viewMenu->addAction(terminalAct);
  viewMenu->addSeparator();
  viewMenu->addAction(showLineEndsAct);
  viewMenu->addAction(codeFoldingAct);
  lexerMenu = viewMenu->addMenu(tr("Lexer"));
  QList<QAction *> * lexerList = &lexers->actions();
  for (int i = 0; i < lexerList->size(); ++i) {
    lexerMenu->addAction(lexerList->at(i));
  }
  
  optionsMenu = menuBar()->addMenu(tr("&Options"));
  lineEndMenu = optionsMenu->addMenu(tr("&Line Endings"));
  lineEndMenu->addAction(lineEndLf);
  lineEndMenu->addAction(lineEndCrLf);
  lineEndMenu->addAction(lineEndCr);
  lineEndMenu->addSeparator();
  lineEndMenu->addAction(convertEndings);
  optionsMenu->addAction(convertIndentAct);
  menuBar()->addSeparator();

  windowMenu = menuBar()->addMenu(tr("&Window"));
  windowMenu->addAction(newWindowAct);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);

  for (int i = 0; i < recentFiles.size(); ++i) {
  	if (recentFiles[i].exists() && recentFiles[i].isFile()) {
  		recentMenu->addAction( recentFiles[i].fileName() )->setStatusTip( recentFiles[i].filePath() );
  	} else {
  		recentFiles.removeAt(i--);
  	}
  }

  recentMenu->menuAction()->setEnabled(!recentFiles.empty());
  connect(recentMenu, SIGNAL(triggered(QAction *)), this, SLOT(openRecent(QAction *)));
}

void MainWindow::createToolBars() {
    QSize buttonSize = QSize(22, 22);
    mainToolBar = addToolBar(tr("Main"));
    mainToolBar->addAction(newAct);
    mainToolBar->addAction(openAct);
    mainToolBar->addAction(saveAct);
    mainToolBar->addAction(saveAsAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(undoAct);
    mainToolBar->addAction(redoAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(cutAct);
    mainToolBar->addAction(copyAct);
    mainToolBar->addAction(pasteAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(prevAct);
    mainToolBar->addAction(nextAct);
    mainToolBar->addSeparator();
    mainToolBar->addAction(terminalAct);
    mainToolBar->setIconSize(buttonSize);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}
