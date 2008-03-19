#ifndef FINDTEXT_H_
#define FINDTEXT_H_

#include "ui_dlgFindText.h"
#include "mainwindow.h"

#include <QDialog>
#include <QSettings>

class dlgFindText: public QDialog, protected Ui_dlgFindText {
  
  Q_OBJECT
  
public:
  dlgFindText(MainWindow * _parent);
  MainWindow * parent;

private slots:
  void doSearch();
  void onTextChange(QString text);
};
  
#endif /*FINDTEXT_H_*/
