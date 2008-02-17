//#include <iostream>
#include <QtGui>
#include "qterminal.h"

QTerminal::QTerminal(QWidget *parent) : QTextEdit(parent) {
    shell = new QProcess();
    shell->setProcessChannelMode(QProcess::MergedChannels);
    QObject::connect(shell, SIGNAL(readyRead()), this, SLOT(readStandardOut()));
    shell->start("bash", QStringList() << "-i", QIODevice::ReadWrite);
}

void QTerminal::readStandardOut() {
    this->append(shell->readAll());    
}

void QTerminal::keyPressEvent(QKeyEvent * event) {
  if (!event->text().isEmpty()) {
    if (event->key() != Qt::Key_Enter) {
      this->append(event->text());
    }
    shell->write(event->text().toAscii().data(), event->text().length());
  }
}
