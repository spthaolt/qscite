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
      openFiles[curDocIdx].edWidget->setFocus();
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
  openFiles[curDocIdx].edWidget->setFocus();
}

bool MainWindow::closeFile() {
  if (tabWidget->count()) {
    if (maybeSave()) {
      QsciScintilla * theEditor = openFiles[curDocIdx].edWidget;

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
  QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select one or more files to open", (curDocIdx < openFiles.size()) ? openFiles[curDocIdx].path : lastDir);
  
  while (fileNames.count()) {
    if (!fileNames.back().isEmpty()) {
      if ((!tabWidget->count()) || (!openFiles[curDocIdx].baseName.isEmpty()) || openFiles[curDocIdx].edWidget->isModified()) {
        createDocument();
      }
      
      loadFile(fileNames.back());
      setCurrentTabTitle();
      
      addRecentFile(fileNames.back());
    }
    
    fileNames.pop_back();
  }
  
  if (!openFiles.empty()) {
    openFiles[curDocIdx].edWidget->setFocus();
    lastDir = openFiles[curDocIdx].path;
  }
}

void MainWindow::openRecent(QAction * src) {
  qDebug() << "openRecent(" << src->statusTip() << ')';
  if ((!tabWidget->count()) || (!openFiles[curDocIdx].baseName.isEmpty()) || openFiles[curDocIdx].edWidget->isModified()) {
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
    QString fileName = QFileDialog::getSaveFileName(this, "Save - QSciTE", (curDocIdx < openFiles.size()) ? openFiles[curDocIdx].path : lastDir);
    
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
      QsciScintilla * curDoc = openFiles[curDocIdx].edWidget;
      QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
      if (newLexer != NULL) {
        QFont curFont = curDoc->font();
        setLexerFont(newLexer, curFont.family(), curFont.pointSize());
        newLexer->setParent(curDoc);
        curDoc->setLexer(newLexer);
        curDoc->recolor();
      }
    }
    return success;
  }
  
  return false;
}

void MainWindow::fontDialog() {
  if (tabWidget->count()) {
  	QsciLexer * lexer = openFiles[curDocIdx].edWidget->lexer();
  	bool ok;
  	
  	if (lexer) {
  	  QFont baseFont = QFontDialog::getFont(&ok, lexer->font(lexer->defaultStyle()));
  	  
  	  if (ok) {
		openFiles[curDocIdx].edWidget->setFont(baseFont);
  	    setLexerFont(lexer, baseFont.family(), baseFont.pointSize());
  	  }
  	} else {
      QFont font = QFontDialog::getFont(&ok, openFiles[curDocIdx].edWidget->font());
      
      if (ok) {
        openFiles[curDocIdx].edWidget->setFont(font);
      }
  	}
  }
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About QSciTE"),
       tr("<b>QSciTE</b> is a clone of SciTE, based on the Scintilla library"
          " and Qt4. It is heavily based on the example code included with"
          " Qscintilla2, and is licensed under the GNU GPL version 2."));
}

void MainWindow::editCopy() {
	if (copyFromTerm) {
		Q_ASSERT(termWidget != NULL);
		termWidget->copy();
	} else if (!openFiles.empty()) {
		openFiles[curDocIdx].edWidget->copy();
	}
}

void MainWindow::editCut() {
	if (!copyFromTerm && !openFiles.empty()) {
		openFiles[curDocIdx].edWidget->cut();
	}
}

void MainWindow::editPaste() {
	if (copyFromTerm) {
		Q_ASSERT(termWidget != NULL);
		termWidget->paste();
	} else if (!openFiles.empty()) {
		openFiles[curDocIdx].edWidget->paste();
	}
}

void MainWindow::undo() {
	if (!openFiles.empty()) {
		openFiles[curDocIdx].edWidget->undo();
	}
}

void MainWindow::redo() {
	if (!openFiles.empty()) {
		openFiles[curDocIdx].edWidget->redo();
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
    openFiles[curDocIdx].edWidget->setEolMode(QsciScintilla::EolMac);
  }
}

void MainWindow::setEolLf () {
  if (openFiles.size() > curDocIdx) {
    openFiles[curDocIdx].edWidget->setEolMode(QsciScintilla::EolUnix);
  }
}

void MainWindow::setEolCrLf () {
  if (openFiles.size() > curDocIdx) {
    openFiles[curDocIdx].edWidget->setEolMode(QsciScintilla::EolWindows);
  }
}

void MainWindow::convertEols () {
  if (openFiles.size() > curDocIdx) {
    openFiles[curDocIdx].edWidget->convertEols(openFiles[curDocIdx].edWidget->eolMode());
  }
}

void MainWindow::setEolVisibility(bool vis) {
  if (openFiles.size() > curDocIdx) {
    openFiles[curDocIdx].edWidget->setEolVisibility(vis);
  }
}

void MainWindow::trayClicked(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger) { // the icon was clicked
    this->setVisible(!this->isVisible());
  }
}
