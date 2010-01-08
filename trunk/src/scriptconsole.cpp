#include <QtGui>
#include <QtDebug>
#include <QString>
#include <Qsci/qsciscintilla.h>
#include <QtScript>

#include "scriptconsole.h"
#include "mainwindow.h"

dlgScriptConsole::dlgScriptConsole(MainWindow * _parent) {
  parent = _parent;
  setupUi(this);
  this->show();
  connect(btnExecute, SIGNAL(clicked()), this, SLOT(executeCommand()));
  this->setAttribute(Qt::WA_DeleteOnClose);
  
  //add some globals into the script engine
  updateDoc();
}

/**
 * Updates the doc variable that is available in the script engine.
 */
void dlgScriptConsole::updateDoc() {
  QScriptValue document = engine.newQObject(parent->getCurDoc());
  QScriptValue window = engine.newQObject(parent);
  engine.globalObject().setProperty("document", document);
  engine.globalObject().setProperty("window", window);
}

/**
 * Executes the command in the command text box.
 */
void dlgScriptConsole::executeCommand() {
  QString cmd = txtCommand->toPlainText();
  QScriptValue result = engine.evaluate(cmd);
  
  if( !(result.isUndefined()) && !(result.isNull()) ) {
    lblError->setText(result.toString());
  } else {
    lblError->setText("Command executed successfully.");
  }
}