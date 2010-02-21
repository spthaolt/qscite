#ifndef _QSCITE_TEXTDISPLAY_H
#define _QSCITE_TEXTDISPLAY_H

#include <QtGui>
#include "ui_frmTextDisplay.h"

class MainWindow;

class TextDisplayPanel : public QWidget, protected Ui_frmTextDisplay {

  Q_OBJECT

public:
  TextDisplayPanel(QWidget * parent = 0, Qt::WindowFlags f = 0);

public slots:
  void populate();
protected slots:
  void on_cbWrapMode_clicked(bool checked);
  void on_cbxWrapMarkers_currentIndexChanged(int idx);
  void on_cbLineNos_clicked(bool checked);
  void on_cbIndentGuides_clicked(bool checked);
  void on_sbIndentSize_valueChanged(int val);
  void on_cbAutoIndent_clicked(bool checked);
  void on_cbxUseTabs_currentIndexChanged(int idx);
  void on_cbxEOLMode_currentIndexChanged(int idx);
  void on_cbxLexer_currentIndexChanged(int idx);
  void applyColor();
  void on_btnReset_clicked();
  void on_btnSaveSettings_clicked();

private:
  MainWindow * parent;
};

#endif
