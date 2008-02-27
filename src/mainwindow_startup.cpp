#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include "mainwindow.h"

void MainWindow::createActions() {
    newAct = new QAction(QIcon(":/images/filenew.svg"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    
    closeAct = new QAction(QIcon(":/images/fileclose.svg"), tr("&Close"), this);
#ifndef Q_WS_MAC
    closeAct->setShortcut(tr("Ctrl+F4"));
#else
	closeAct->setShortcut(tr("Ctrl+W"));
#endif
    closeAct->setStatusTip(tr("Close the current file"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(closeFile()));

    openAct = new QAction(QIcon(":/images/fileopen.svg"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/filesave.svg"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/filesaveas.svg"), tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit QSciTE"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/editcut.svg"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(editCut()));

    copyAct = new QAction(QIcon(":/images/editcopy.svg"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(editCopy()));

    pasteAct = new QAction(QIcon(":/images/editpaste.svg"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(editPaste()));
    
    prefsAct = new QAction(QIcon(":/images/configure.svg"), tr("P&references"), this);
    // TODO: set shortcut, tip, etc.
    connect(prefsAct, SIGNAL(triggered()), this, SLOT(globalPrefs()));
    
    fontAct = new QAction(QIcon(":/images/font.svg"), tr("&Font"), this);
    //fontAct->setShortcut(tr(""))
    fontAct->setStatusTip(tr("Set the display font."));
    
    connect(fontAct, SIGNAL(triggered()), this, SLOT(fontDialog()));
    
    terminalAct = new QAction(QIcon(":/images/openterm.svg"), tr("Terminal"), this);
    connect(terminalAct, SIGNAL(triggered()), this, SLOT(toggleTerminal()));
    
    textDisplayAct = new QAction(QIcon(":/images/font.svg"), tr("Text &Display..."), this);
    connect(textDisplayAct, SIGNAL(triggered()), this, SLOT(textDisplay()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show QSciTE's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
 	fileMenu->addSeparator();
    fileMenu->addAction(openAct);
    recentMenu = fileMenu->addMenu(tr("Open &Recent"));
    fileMenu->addAction(closeAct);
 	fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(prefsAct);
    
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(textDisplayAct);
    viewMenu->addAction(terminalAct);

    menuBar()->addSeparator();

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
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(terminalAct);
    //fileToolBar->addAction(closeAct);
    fileToolBar->setIconSize(QSize(22, 22));

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->setIconSize(QSize(22, 22));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}
