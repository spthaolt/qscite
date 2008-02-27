#ifndef QTERMINAL_H_
#define QTERMINAL_H_
#include <QTextEdit>
class QProcess;

class QTerminal : public QTextEdit {
    Q_OBJECT
public:
    QTerminal(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~QTerminal();
    void keyPressEvent(QKeyEvent * event);
    void changeDir(const QString & dir);

private slots:
    void readStandardOut();
    void readStandardErr();
private:
    QProcess * shell;
    int inputCharCount;
    QTextCursor curCursorLoc;
};

#endif /*QTERMINAL_H_*/
