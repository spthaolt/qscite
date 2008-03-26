#include <QApplication>
#include <QVector>
#include <QStringList>

#include "launcher.h"
#include "mainwindow.h"

Launcher::Launcher (QStringList argv, QApplication * _app) {
  app = _app;
  createFirstWindow(argv);
}

void Launcher::createFirstWindow(QStringList _argv) {
  MainWindow * window = new MainWindow(_argv, this);
  app->installEventFilter(window);
  window->show();
  windows.push_back(window);
}

void Launcher::createNewWindow() {
  QStringList _argv;
  MainWindow * window = new MainWindow(_argv, this);
  app->installEventFilter(window);
  window->show();
  windows.push_back(window);
}