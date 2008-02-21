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

void MainWindow::readSettings() {
	QSettings settings;

	if (settings.value("version", 0).toInt() < 1) {
#ifdef QSCITE_DEBUG
		std::cout << "Using default preferences" << std::endl;
#endif
		writeDefaultSettings(settings);
	}

	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
	
	if (settings.value("maximized", false).toBool()) {
		setWindowState(windowState() | Qt::WindowMaximized);
	}
}

void MainWindow::writeSettings() {
  QSettings settings;
  
  if (settings.value("saveWindowGeometry", false).toBool()) {
    if (isMaximized()) {
      settings.setValue("maximized", true);
    } else {
      settings.setValue("pos", pos());
      settings.setValue("size", size());
	  settings.setValue("maximized", false);
    }
  }
}

void MainWindow::globalPrefs() {
	MainPrefsDialog * dlgPrefs = new MainPrefsDialog(this, Qt::Sheet);

	connect(dlgPrefs, SIGNAL(accepted()), this, SLOT(prefsWereChanged()), Qt::QueuedConnection);
	connect(dlgPrefs, SIGNAL(prefsWereReset()), this, SLOT(prefsWereChanged()));
	connect(dlgPrefs, SIGNAL(finished(int)), this, SLOT(reapPrefs()));
	
	dlgPrefs->show();
}

void MainWindow::prefsWereChanged() {
	if (termWidget != NULL) {
		applyPrefsToTerminal(termWidget);
	}
	if (curDoc != NULL) {
		applySettingsToDoc(curDoc);
	}
}

void MainWindow::reapPrefs() {
	sender()->deleteLater();
}

