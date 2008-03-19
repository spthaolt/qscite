#include <QtGui>
#include <QtDebug>
#include <QString>
#include <Qsci/qsciscintilla.h>

#include "findtext.h"
#include "mainwindow.h"

dlgFindText::dlgFindText(MainWindow * _parent) {
  parent = _parent;
  setupUi(this);
  this->show();
  connect(btnFind, SIGNAL(clicked()), this, SLOT(doSearch()));
  connect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(onTextChange(QString)));
  this->setAttribute(Qt::WA_DeleteOnClose);
}

void dlgFindText::doSearch() {
  if (btnFind->text() == "&Find") {
    QString expr = comboBox->currentText();
    bool regex = chkRegEx->isChecked();
    bool caseSens = chkCase->isChecked();
    bool wholeWord = chkWhole->isChecked();
    bool forward = !chkBackwards->isChecked();
    bool result = parent->getCurDoc()->findFirst(expr, regex, caseSens, wholeWord, false, forward);
    
    if (result) {
      btnFind->setText("&Find Next");
    }
  } else {
    bool result = parent->getCurDoc()->findNext();
    
    if (!result) {
      btnFind->setText("&Find");
    }
  }
}

void dlgFindText::onTextChange(QString text) {
  text = ""; //don't care about the string, but making compiler happy
  btnFind->setText("&Find");
}
