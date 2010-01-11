#ifndef FINDREPLACE_H_
#define FINDREPLACE_H_

#include "ui_dlgFindReplace.h"
#include "mainwindow.h"

#include <QDialog>
#include <QSettings>
#include <QEvent>

class dlgFindReplace: public QDialog, protected Ui_dlgFindReplace {
  
  Q_OBJECT
  
public:
  dlgFindReplace(MainWindow * _parent);
  MainWindow * parent;
  void setFind();

private slots:
  void doSearch();
  void doReplace();
  void onTextChange(QString text);
  void keyPressEvent(QKeyEvent * event);
};
  
#endif /*FINDREPLACE_H_*/
