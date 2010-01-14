#ifndef QSCITEEDITOR_H_
#define QSCITEEDITOR_H_
#include <Qsci/qsciscintilla.h>

class QsciteEditor : public QsciScintilla {

    Q_OBJECT
  
  public:
    QsciteEditor();
  
  protected:
    void keyPressEvent(QKeyEvent * event);
    void unindentHandler();

};
    
#endif /*QSCITEEDITOR_H_*/
