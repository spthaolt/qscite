#include <QtWidgets>
#include <QtDebug>
#include <QString>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjavascript.h>
#include <QtScript>

#include "scriptconsole.h"
#include "mainwindow.h"
#include "prefs.h"

dlgScriptConsole::dlgScriptConsole(MainWindow * _parent) {
  parent = _parent;
  setupUi(this);
  this->show();
  connect(btnExecute, SIGNAL(clicked()), this, SLOT(executeCommand()));
  this->setAttribute(Qt::WA_DeleteOnClose);
  
  engine = &(parent->scriptEngine);
  
  QsciteEditor * curDoc = new QsciteEditor(parent);
  
  /*qDebug() << "ready to create scintilla";
  doc = new QsciteEditor(verticalLayout);*/
  curDoc->setUtf8(true);
  
  qDebug() << "apply settings to scintilla";
  applySettingsToDoc(curDoc);
  
  qDebug() << "set the lexer to javascript";
  QsciLexerJavaScript * lexer = new QsciLexerJavaScript();
  curDoc->setLexer(lexer);
  
  qDebug() << "add scintilla to window";
  verticalLayout->insertWidget(0, curDoc);
  doc = curDoc;
  doc->show();
  qDebug() << "done";
}

dlgScriptConsole::~dlgScriptConsole() {
  delete doc;
}

/**
 * Executes the command in the command text box.
 */
void dlgScriptConsole::executeCommand() {
  QString cmd = doc->text(); //txtCommand->toPlainText();
  QScriptValue result = engine->evaluate(cmd);
  
  if( !(result.isUndefined()) && !(result.isNull()) ) {
    lblError->setText(result.toString());
  } else {
    lblError->setText("Command executed successfully.");
  }
}
