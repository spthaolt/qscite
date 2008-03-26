#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include <QVector>

class QApplication;
class QStringList;
class MainWindow;

class Launcher {
  private:
    QApplication * app;
    QVector<MainWindow *> windows;
    void createFirstWindow(QStringList _argv);
  
  public:
    Launcher (QStringList argv, QApplication * _app);
    void createNewWindow();
    
  public slots:
    void QuitApplication();
};

#endif /*LAUNCHER_H_*/
