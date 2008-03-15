#include <QtGui>
#include <QtDebug>
#include <QString>
#include <Qsci/qsciscintilla.h>

#include "findtext.h"

dlgFindText::dlgFindText(QWidget * parent, QsciScintilla * _doc) {
  setupUi(this);
  this->show();
  connect(btnFind, SIGNAL(clicked()), this, SLOT(doSearch()));
  curDoc = _doc;
}

void dlgFindText::doSearch() {
  QString expr = comboBox->currentText();
  bool regex = chkRegEx->isChecked();
  bool caseSens = chkCase->isChecked();
  bool wholeWord = chkWhole->isChecked();
  bool forward = !chkBackwards->isChecked();
  curDoc->findFirst(expr, regex, caseSens, wholeWord, false, forward);
}


