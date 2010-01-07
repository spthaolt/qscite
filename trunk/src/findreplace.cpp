#include <QtGui>
#include <QtDebug>
#include <QString>
#include <Qsci/qsciscintilla.h>

#include "findreplace.h"
#include "mainwindow.h"

dlgFindReplace::dlgFindReplace(MainWindow * _parent) {
  parent = _parent;
  setupUi(this);
  this->show();
  connect(btnFind, SIGNAL(clicked()), this, SLOT(doSearch()));
  connect(btnReplace, SIGNAL(clicked()), this, SLOT(doReplace()));
  connect(cboFind, SIGNAL(editTextChanged(QString)), this, SLOT(onTextChange(QString)));
  this->setAttribute(Qt::WA_DeleteOnClose);
  
  //if there is text highlighted then assume that is what they want to search for
  if( parent->getCurDoc()->hasSelectedText() ) {
    QString text = parent->getCurDoc()->selectedText();
    cboFind->setEditText(text);
  }
}

void dlgFindReplace::setFind() {
  grpReplace->hide();
  btnReplace->hide();
  this->resize(10,10);
  this->adjustSize();
}

void dlgFindReplace::doSearch() {
  if (btnFind->text() == "&Find") {
    QString expr = cboFind->currentText();
    bool regex = chkRegEx->isChecked();
    bool caseSens = chkCase->isChecked();
    bool wholeWord = chkWhole->isChecked();
    bool forward = !chkBackwards->isChecked();
    bool result = parent->getCurDoc()->findFirst(expr, regex, caseSens, wholeWord, true, forward);
    
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

void dlgFindReplace::doReplace() {
  doSearch();
  QString expr = cboReplace->currentText();
  
  parent->getCurDoc()->replace(expr);
}

void dlgFindReplace::onTextChange(QString text) {
  text = ""; //don't care about the string, but making compiler happy
  btnFind->setText("&Find");
}
