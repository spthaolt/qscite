#ifndef QTERMINAL_H_
#define QTERMINAL_H_
#include <QTextEdit>
class QProcess;

class QTerminal : public QTextEdit {
    Q_OBJECT
public:
    QTerminal(QWidget * parent = 0);
    void keyPressEvent(QKeyEvent * event);

private slots:
    void readStandardOut();
    
private:
    QProcess * shell;
    
};

#endif /*QTERMINAL_H_*/
