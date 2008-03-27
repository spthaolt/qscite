#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include <QVector>
#include <QObject>
#include <QSystemTrayIcon>

class QApplication;
class QStringList;
class MainWindow;
class QAction;
class QMenu;

class Launcher : public QObject {
  Q_OBJECT
  
  private:
    QApplication * app;
    QVector<MainWindow *> windows;
    void createFirstWindow(QStringList _argv);
    void createTrayIcon();
    void createActions();
    QSystemTrayIcon * trayIcon;
    
    QMenu * trayIconMenu;
    
    /* Actions */
    QAction * minimizeAction;
    QAction * maximizeAction;
    QAction * restoreAction;
    QAction * quitAction;
  
  public:
    Launcher (QStringList argv, QApplication * _app);
    ~Launcher ();
    void createNewWindow();
    
  private slots:
    void quitApplication();
    void trayClicked(QSystemTrayIcon::ActivationReason reason);
    
};

#endif /*LAUNCHER_H_*/
