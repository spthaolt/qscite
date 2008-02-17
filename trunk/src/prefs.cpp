#include "prefs.h"
#include "lexer_utils.h"

#include <QSettings>

MainPrefsDialog::MainPrefsDialog(QWidget * parent, Qt::WindowFlags f) :
	QDialog(parent, f) 
{
	setupUi(this);
}


void writeDefaultSettings(QSettings & settings) {
  settings.remove("");

  settings.beginGroup("extensions");
  writeDefaultExtensions(settings);
  settings.endGroup();

  settings.beginGroup("magic");
  writeDefaultMagic(settings);
  settings.endGroup();

  settings.setValue("saveWindowGeometry", true);
}
