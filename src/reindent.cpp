#include <QtGui>
#include <Qsci/qsciscintilla.h>

#include "mainwindow.h"
#include "textdisplay.h"
#include "ui_dlgConvertIndent.h"

namespace {
  class DlgConvertIndent : public QDialog, public Ui_dlgConvertIndent {
    public:
    DlgConvertIndent( QsciScintilla * doc = 0,
                      QWidget * parent = 0,
                      Qt::WindowFlags f = 0) : QDialog(parent, f)
    {
      setupUi(this);
      
      if (doc != NULL) {
        sbOldWidth->setValue(doc->tabWidth());
        sbNewWidth->setValue(doc->tabWidth());
        cbLeaveTabs->setChecked(!doc->indentationsUseTabs());
      }
    }
  };
}

void MainWindow::convertIndentation() {
  QsciScintilla * curDoc;
  if (curDocIdx < openFiles.size()) {
    curDoc = openFiles[curDocIdx].edWidget;
  } else {
    return;
  }
  
  DlgConvertIndent * dlg = new DlgConvertIndent(curDoc, this, Qt::Sheet);
  if (dlg->exec()) {
    QString theText = curDoc->text();
    if (dlg->cbExpandTabs->isChecked()) {
      theText.replace('\t', QString().fill(' ', dlg->sbOldWidth->value()));
    }
    theText.replace(QString().fill(' ', dlg->sbOldWidth->value()), "\t");
    if (!dlg->cbLeaveTabs->isChecked()) {
      theText.replace('\t', QString().fill(' ', dlg->sbNewWidth->value()));
    }
    curDoc->setText(theText);
    curDoc->setTabWidth(dlg->sbNewWidth->value());
    curDoc->setIndentationWidth(dlg->sbNewWidth->value());
    if (textSettingsWidget != NULL) {
      textSettingsWidget->populate();
    }
  }
  delete dlg;
}
