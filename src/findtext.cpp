#include <QtGui>
#include <QtDebug>
#include <QString>
#include <Qsci/qsciscintilla.h>

#include "findtext.h"

dlgFindText::dlgFindText(QWidget * parent, QsciScintilla * _doc) {
  setupUi(this);
  this->show();
  connect(btnFind, SIGNAL(clicked()), this, SLOT(doSearch()));
  connect(comboBox, SIGNAL(textChanged(QString)), this, SLOT(onTextChange(QString)));
  connect(btnClose, SIGNAL(clicked()), this, SLOT(closeMe()));
  curDoc = _doc;
}

void dlgFindText::doSearch() {
  if (btnFind->text() == "&Find") {
    QString expr = comboBox->currentText();
    bool regex = chkRegEx->isChecked();
    bool caseSens = chkCase->isChecked();
    bool wholeWord = chkWhole->isChecked();
    bool forward = !chkBackwards->isChecked();
    bool result = curDoc->findFirst(expr, regex, caseSens, wholeWord, false, forward);
    if (result) {
      btnFind->setText("&Find Next");
    }
  } else {
    bool result = curDoc->findNext();
    if (!result) {
      btnFind->setText("&Find");
    }
  }
}

void dlgFindText::onTextChange(QString text) {
  text = ""; //don't care about the string, but making compiler happy
  btnFind->setText("&Find");
}

void dlgFindText::closeMe() {
  emit(closed(this));
}
