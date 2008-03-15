#ifndef FINDTEXT_H_
#define FINDTEXT_H_

#include "ui_dlgFindText.h"
#include "mainwindow.h"

#include <QDialog>
#include <QSettings>
class QsciScintilla;

class dlgFindText: public QDialog, protected Ui_dlgFindText {
  
  Q_OBJECT
  
public:
  dlgFindText(QWidget * parent, QsciScintilla * _doc);
  QsciScintilla * curDoc;
signals:
  
private slots:
  void doSearch();
};
  
#endif /*FINDTEXT_H_*/
