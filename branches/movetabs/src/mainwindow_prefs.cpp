#include <QtGui>
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

void MainWindow::readSettings() {
	QSettings settings;

	if (settings.value("version", 0).toInt() < 1) {
		qDebug() << "Using default preferences";
		writeDefaultSettings(settings);
	}

	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);

	if (settings.value("maximized", false).toBool()) {
		setWindowState(windowState() | Qt::WindowMaximized);
	}

	for (int i = 0, j = settings.beginReadArray("recentFiles"); i < j; ++i) {
		settings.setArrayIndex(i);
		QFileInfo curFile(settings.value("info").toString());
		if (curFile.exists() && curFile.isFile()) {
			recentFiles.push_back(curFile);
		}
	}

	settings.endArray();
	lastDir = settings.value("lastDir", QDir::homePath()).toString();

	setWindowOpacity(QSettings().value("wndOpacity", 1.0).toDouble());

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

  if (settings.value("recentFileCount", 0).toInt() > 0) {
    settings.beginWriteArray("recentFiles");
    for (int i = 0; i < recentFiles.size(); ++i) {
    	if (recentFiles[i].exists() && recentFiles[i].isFile()) {
			settings.setArrayIndex(i);
			settings.setValue("info", recentFiles[i].canonicalFilePath());
		} else {
			recentFiles.removeAt(i--);
		}
    }
    settings.endArray();
  }

  settings.setValue("lastDir", lastDir);
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
	if (openFiles.size() > curDocIdx) {
		applySettingsToDoc(getCurDoc());
		if (textSettingsWidget != NULL) {
			textSettingsWidget->populate();
		}
	}
	setWindowOpacity(QSettings().value("wndOpacity", 1.0).toDouble());
}

void MainWindow::reapPrefs() {
	sender()->deleteLater();
}

void MainWindow::textDisplay() {
	if (textSettingsWidget == NULL) {
		textSettingsWidget = new TextDisplayPanel(this,
#ifdef Q_WS_MAC
			Qt::Drawer
#else
			Qt::Tool
#endif
		);

		connect(textSettingsWidget, SIGNAL(destroyed()), this, SLOT(textDisplayDeleted()));
		connect(tabWidget, SIGNAL(currentChanged(int)), textSettingsWidget, SLOT(populate()));

		// move the dialog to the center of the window before making it visible
		int x = this->pos().x() + (this->width() / 2) - (textSettingsWidget->width() / 2);
		int y = this->pos().y() + (this->height() / 2) - (textSettingsWidget->height() / 2);
    textSettingsWidget->move(x, y);
		textSettingsWidget->show();
	} else {
		textSettingsWidget->close();
	}
}

void MainWindow::textDisplayDeleted() {
	textSettingsWidget = NULL;
}

