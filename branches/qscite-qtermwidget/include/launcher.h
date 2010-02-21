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
  void createNewWindow(QStringList _argv = QStringList());

private slots:
  void trayClicked(QSystemTrayIcon::ActivationReason reason);
  void windowClosed();

public slots:
  void quitApplication();

};

#endif /*LAUNCHER_H_*/
