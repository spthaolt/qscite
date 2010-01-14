#include <QtGui>
#include <QtDebug>
#include "qsciteeditor.h"
#include <Qsci/qsciscintilla.h>

QsciteEditor::QsciteEditor() : QsciScintilla() { }

void QsciteEditor::unindentHandler() {
  if (hasSelectedText()) {
    int lineFrom(0), lineTo(0), indexFrom(0), indexTo(0);
    getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
    
    for (int i = lineFrom; i <= lineTo; ++i) {
      unindent(i);
    }
  } else {
    int line(0), index(0);
    getCursorPosition(&line, &index);
    unindent(line);
    // attempt to return the cursor to it's original location, correcting for new indentation.
    setCursorPosition(line, index - indentationWidth());
  }
}

void QsciteEditor::keyPressEvent(QKeyEvent * event) {
  // Shift+Tab is not supported by QScintilla, so we have to do that ourselves...
  if (event->key() == Qt::Key_Backtab) {
    unindentHandler();
  }
 
  QsciScintilla::keyPressEvent(event);
}
