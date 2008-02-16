#ifndef _QSCITE_DLG_MAIN_PREFS_H
#define _QSCITE_DLG_MAIN_PREFS_H

#include "ui_dlgPrefsUI.h"

#include <QDialog>

class MainPrefsDialog: public QDialog, protected Ui_QSciTEPrefs {
	
	Q_OBJECT
	
  public:
	MainPrefsDialog(QWidget * parent=0, Qt::WindowFlags f=0);

};

#endif
