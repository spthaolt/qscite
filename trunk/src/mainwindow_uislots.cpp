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
#include "findreplace.h"
#include "scriptconsole.h"

#ifdef _WIN32
  #include "qterminal.h"
#else
  #include "qterminal_pty.h"
#endif

void MainWindow::toggleTerminal(bool alive) {
  if (alive) { // we were called by a dock window reporting itself visible
    return;
  }
  QSettings settings;

  if (termWidget != NULL) {
    if (!termInDrawer) {
      termWidget->parent()->deleteLater();
    }

    qDebug() << "Closing terminal";
    termWidget->disconnect();
    termWidget->deleteLater();
    termWidget = NULL;

    if (openFiles.size() > curDocIdx) {
      getCurDoc()->setFocus();
    }

    copyFromTerm = false;
  } else {
    qDebug() << "Opening terminal";
    termWidget = new QTerminal(this);
    applyPrefsToTerminal(termWidget);
    if (openFiles.size() > curDocIdx && !openFiles[curDocIdx].fullName.isEmpty()) {
    	termWidget->changeDir(openFiles[curDocIdx].path);
    }

    if (termInDrawer) {
      termWidget->setWindowFlags(Qt::Drawer);
      termWidget->show();
    } else {
      QDockWidget * termDock = new QDockWidget(tr("QSciTE Terminal"), this);
      termDock->setWidget(termWidget);
      termDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      this->addDockWidget(Qt::BottomDockWidgetArea, termDock);
    }

    connect(termWidget, SIGNAL(shellExited()), this, SLOT(toggleTerminal()));
    connect(termWidget, SIGNAL(copyAvailable(bool)), this, SLOT(updateCopyAvailable(bool)));
    termWidget->setFocus();
    copyFromTerm = true;
  }
}

void MainWindow::newFile() {
  createDocument();
  getCurDoc()->setFocus();
}

bool MainWindow::closeFile() {
  if (tabWidget->count()) {
    if (maybeSave()) {
      QsciScintilla * theEditor = getCurDoc();

      openFiles.erase(openFiles.begin() + curDocIdx);
      tabWidget->removeTab(curDocIdx);

      delete theEditor;

      if (tabWidget->count() == 0) { // out of tabs
        setWindowTitle(tr("QSciTE"));
        setWindowModified(false);
        if (textSettingsWidget != NULL) {
          textSettingsWidget->setEnabled(false);
		}
      }
      return true;
    }
  }

  return false;
}

void MainWindow::open() {
  QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select one or more files to open", (curDocIdx < openFiles.size() && !openFiles[curDocIdx].path.isEmpty()) ? openFiles[curDocIdx].path : lastDir);

  while (fileNames.count()) {
    if (!fileNames.back().isEmpty()) {
      if ((!tabWidget->count()) || (!openFiles[curDocIdx].baseName.isEmpty()) || getCurDoc()->isModified()) {
        createDocument();
      }

      loadFile(fileNames.back());
      setCurrentTabTitle();

      addRecentFile(fileNames.back());
    }

    fileNames.pop_back();
  }

  if (!openFiles.empty()) {
    getCurDoc()->setFocus();
    lastDir = openFiles[curDocIdx].path;
  }
}

void MainWindow::openRecent(QAction * src) {
  qDebug() << "openRecent(" << src->statusTip() << ')';
  if ((!tabWidget->count()) || (!openFiles[curDocIdx].baseName.isEmpty()) || getCurDoc()->isModified()) {
	createDocument();
  }

  loadFile(src->statusTip());
  setCurrentTabTitle();
}

bool MainWindow::save() {
  if (tabWidget->count()) {
    if (openFiles[curDocIdx].fullName.isEmpty()) {
      return saveAs();
    } else {
      return saveFile(openFiles[curDocIdx].fullName);
    }
  }

  // make the compiler happy :)
  return false;
}

bool MainWindow::saveAs() {
  if (tabWidget->count()) {
    QString fileName = QFileDialog::getSaveFileName(this, "Save - QSciTE", (curDocIdx < openFiles.size() && !openFiles[curDocIdx].path.isEmpty()) ? openFiles[curDocIdx].path : lastDir);

    if (fileName.isEmpty()) {
      return false;
    }

    bool success = saveFile(fileName);
    if (success) {
      addRecentFile(fileName);
      openFiles[curDocIdx].setPathName(fileName);
      setCurrentTabTitle();
      setWindowTitleForFile(openFiles[curDocIdx].baseName);
      lastDir = openFiles[curDocIdx].path;
      QsciScintilla * curDoc = getCurDoc();
      QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
      if (newLexer != NULL) {
/*
        QFont curFont = curDoc->font();
        setLexerFont(newLexer, curFont.family(), curFont.pointSize());
        newLexer->setParent(curDoc);
        curDoc->setLexer(newLexer);
        curDoc->recolor();
*/
        setLexer(newLexer);
      }
    }
    return success;
  }

  return false;
}

void MainWindow::fontDialog() {
  if (tabWidget->count()) {
  	QsciLexer * lexer = getCurDoc()->lexer();
  	bool ok;

  	if (lexer) {
  	  QFont baseFont = QFontDialog::getFont(&ok, lexer->font(lexer->defaultStyle()));

  	  if (ok) {
  	    getCurDoc()->setFont(baseFont);
  	    setLexerFont(lexer, baseFont.family(), baseFont.pointSize());
  	  }
  	} else {
      QFont font = QFontDialog::getFont(&ok, getCurDoc()->font());

      if (ok) {
        getCurDoc()->setFont(font);
      }
  	}
  }
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About QSciTE"),
       tr("<b>QSciTE</b> is a clone of SciTE, based on the Scintilla library"
          " and Qt4. It was originally based on the example code included with"
          " Qscintilla2, however it has grown significantly beyond that codebase."
    	  " QSciTE is licensed under the GNU GPL version 2."));
}

void MainWindow::editCopy() {
	if (copyFromTerm) {
		Q_ASSERT(termWidget != NULL);
		termWidget->copy();
	} else if (!openFiles.empty()) {
	  getCurDoc()->copy();
	}
}

void MainWindow::editCut() {
	if (!copyFromTerm && !openFiles.empty()) {
	  getCurDoc()->cut();
	}
}

void MainWindow::editPaste() {
	if (copyFromTerm) {
		Q_ASSERT(termWidget != NULL);
		termWidget->paste();
	} else if (!openFiles.empty()) {
	  getCurDoc()->paste();
	}
}

void MainWindow::undo() {
	if (!openFiles.empty()) {
	  getCurDoc()->undo();
	}
}

void MainWindow::redo() {
	if (!openFiles.empty()) {
	  getCurDoc()->redo();
	}
}

void MainWindow::nextDoc() {
  int newIdx = curDocIdx + 1;

  if (newIdx >= tabWidget->count()) {
    newIdx = 0;
  }

  changeTabs(newIdx);
}

void MainWindow::prevDoc() {
  int newIdx = curDocIdx - 1;

  if (newIdx < 0) {
    newIdx = tabWidget->count() - 1 ;
  }

  changeTabs(newIdx);
}

void MainWindow::setEolCr () {
  if (openFiles.size() > curDocIdx) {
    getCurDoc()->setEolMode(QsciScintilla::EolMac);
  }
}

void MainWindow::setEolLf () {
  if (openFiles.size() > curDocIdx) {
    getCurDoc()->setEolMode(QsciScintilla::EolUnix);
  }
}

void MainWindow::setEolCrLf () {
  if (openFiles.size() > curDocIdx) {
    getCurDoc()->setEolMode(QsciScintilla::EolWindows);
  }
}

void MainWindow::convertEols () {
  if (openFiles.size() > curDocIdx) {
    getCurDoc()->convertEols(getCurDoc()->eolMode());
  }
}

void MainWindow::setEolVisibility(bool vis) {
  if (openFiles.size() > curDocIdx) {
    getCurDoc()->setEolVisibility(vis);
  }
}

void MainWindow::lexerMenuChanged() {
  setLexer(lexers->checkedAction()->text());
}

void MainWindow::toggleFolding() {
  QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!getCurDoc()->folding()) * 5);

  if (!state) {
    // unfold all code before turning off folding
    getCurDoc()->foldAll(false);
  }

  getCurDoc()->setFolding(state);
}

/**
 * Creates a Find Dialog and displays it
 */
void MainWindow::showFindDialog() {
  showReplaceDialog();
  replaceDialog->setFind();
}

/**
 * Function to call when replace dialog is closed.
 */
void MainWindow::replaceDialogClosed() {
  qDebug() << "replace dialog closed";
  replaceDialog = NULL;
  this->activateWindow();
}

/**
 * Creates a Replace Dialog and displays it.
 */
void MainWindow::showReplaceDialog() {
  if( replaceDialog == NULL ) {
    qDebug() << "creating replace dialog";
    replaceDialog = new dlgFindReplace(this);
    //make sure that we call replaceDialogClosed() when Replace Dialog is closed.
    connect(replaceDialog, SIGNAL(destroyed()), this, SLOT(replaceDialogClosed()));
    //make sure that replace dialog is closed if QScite is closed.
    connect(this, SIGNAL(destroyed()), replaceDialog, SLOT(close()));
  } else {
    qDebug() << "raising replace dialog";
    replaceDialog->activateWindow();
    replaceDialog->raise();
  }
}

/**
 * Function to call when script console is closed.
 */
void MainWindow::scriptConsoleClosed() {
  qDebug() << "script console closed";
  scriptConsole = NULL;
  this->activateWindow();
}

/**
 * Creates a Script Console and displays it.
 */
void MainWindow::showScriptConsole() {
  if( scriptConsole == NULL ) {
    qDebug() << "creating script console";
    scriptConsole = new dlgScriptConsole(this);
    //make sure that we call scriptConsoleClosed() when Script Console is closed.
    connect(scriptConsole, SIGNAL(destroyed()), this, SLOT(scriptConsoleClosed()));
    //make sure that Script Console is closed if QScite is closed.
    connect(this, SIGNAL(destroyed()), scriptConsole, SLOT(close()));
  } else {
    qDebug() << "raising script console";
    scriptConsole->activateWindow();
    scriptConsole->raise();
  }
}

void MainWindow::newWindow() {
  launcher->createNewWindow();
}
