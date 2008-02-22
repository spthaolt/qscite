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

void MainWindow::toggleTerminal() {
  QSettings settings;
  
  if (termWidget != NULL) {
#ifdef QSCITE_DEBUG
    std::cout << "Closing terminal" << std::endl;
#endif
    termWidget->disconnect();
    termWidget->deleteLater();
    termWidget = NULL;
    
    if (curDoc) {
      curDoc->setFocus();
    }
    
    copyFromTerm = false;
  } else {
#ifdef QSCITE_DEBUG
  	std::cout << "Opening terminal" << std::endl;
#endif
    termWidget = new QTerminal(this);
    applyPrefsToTerminal(termWidget);
    
    if (termInDrawer) {
      termWidget->setWindowFlags(Qt::Drawer);
      termWidget->show();
    } else {
      ((QSplitter *)centralWidget())->addWidget(termWidget);
    }
    
    connect(termWidget, SIGNAL(shellExited()), this, SLOT(toggleTerminal()));
    connect(termWidget, SIGNAL(copyAvailable(bool)), this, SLOT(updateCopyAvailable(bool)));
    termWidget->setFocus();
    copyFromTerm = true;
  }
}

void MainWindow::newFile() {
  createDocument();
  curDoc->setFocus();
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
        setWindowModified(false);
        curDoc = NULL;
      }
      
      return true;
    }
  }
  
  return false;
}

void MainWindow::open() {
  QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select one or more files to open");
  
  while (fileNames.count()) {
    if (!fileNames.back().isEmpty()) {
      if ((!tabWidget->count()) || (!curFile.isEmpty()) || modified[tabWidget->currentIndex()]) {
        createDocument();
      }
      
      loadFile(fileNames.back());
      setCurrentTabTitle();
  	  curDoc->setFocus();
    }
    
    fileNames.pop_back();
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
  	bool ok;
  	
  	if (lexer) {
  	  QFont baseFont = QFontDialog::getFont(&ok, lexer->font(lexer->defaultStyle()));
  	  
  	  if (ok) {
  	    setLexerFont(lexer, baseFont.family(), baseFont.pointSize());
  	  }
  	} else {
      QFont font = QFontDialog::getFont(&ok, curDoc->font());
      
      if (ok) {
        curDoc->setFont(font);
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
		termWidget->copy();
	} else {
		curDoc->copy();
	}
}

void MainWindow::editCut() {
	if (!copyFromTerm) {
		curDoc->cut();
	}
}

void MainWindow::editPaste() {
	if (!copyFromTerm) {
		curDoc->paste();
	} else {
		termWidget->paste();
	}
}
