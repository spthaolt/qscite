#ifndef SCRIPTCONSOLE_H_
#define SCRIPTCONSOLE_H_

#include "ui_dlgScriptConsole.h"
#include "mainwindow.h"

#include <QDialog>
#include <QSettings>
#include <QtScript>
#include <Qsci/qsciscintilla.h>

class dlgScriptConsole: public QDialog, protected Ui_dlgScript {

  Q_OBJECT

public:
  dlgScriptConsole(MainWindow * _parent);
  ~dlgScriptConsole();

  MainWindow * parent;

protected slots:
  void executeCommand();

private:
  QScriptEngine * engine;
  QsciteEditor * doc;
};

#endif /*SCRIPTCONSOLE_H_*/
