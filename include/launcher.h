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
    QStringList argv_store;
    QVector<MainWindow *> windows;
    void createTrayIcon();
    void createActions();
    QSystemTrayIcon * trayIcon;

    QMenu * trayIconMenu;
    QMenu * mac_menu;

    /* Actions */
    QAction * minimizeAction;
    QAction * maximizeAction;
    QAction * restoreAction;
    QAction * quitAction;
    QAction * newWindow;


  public:
    Launcher (QStringList argv, QApplication * _app);
    ~Launcher ();
    void createNewWindow(QStringList _argv = QStringList());

  private slots:
    void trayClicked(QSystemTrayIcon::ActivationReason reason);
    void windowClosed();

  public slots:
    void quitApplication();
    void makeNewWindow();

};

#endif /*LAUNCHER_H_*/
