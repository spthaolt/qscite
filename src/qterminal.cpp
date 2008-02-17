//#include <iostream>
#include <QtGui>
#include "qterminal.h"

QTerminal::QTerminal(QWidget *parent) : QTextEdit(parent) {
  shell = new QProcess();
  shell->setProcessChannelMode(QProcess::MergedChannels);
  QObject::connect(shell, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOut()));
  QObject::connect(shell, SIGNAL(readyReadStandardError()), this, SLOT(readStandardErr()));
  shell->start("bash", QStringList() << "-i", QIODevice::ReadWrite);
  // This var protects against mouse interference with the cursor
  curCursorLoc = this->textCursor();
  inputCharCount = 0;
}

QTerminal::~QTerminal() {
  shell->kill();
  delete shell;
}

void QTerminal::readStandardOut() {
  this->insertPlainText(shell->readAllStandardOutput());
  this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
}

void QTerminal::readStandardErr() {
  // TODO: format stderr output differently than stdout
  this->insertPlainText(shell->readAllStandardError());
  this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
}

void QTerminal::keyPressEvent(QKeyEvent * event) {
  int key = event->key();
  
  this->setTextCursor(curCursorLoc);
  
  // Keep QTextEdit in sync with shell as much as possible...
  if (key != Qt::Key_Backspace) {
    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
      inputCharCount = 0;
      QTextEdit::keyPressEvent(event);
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
      // TODO: see if there's a way to hack the bash history in here...
    } else if (key == Qt::Key_Left) {
      if (inputCharCount) {
        --inputCharCount;
        QTextEdit::keyPressEvent(event);
      } else {
        QApplication::beep();
      }
    } else if (key == Qt::Key_Right) {
      QTextCursor cursor = this->textCursor();
      
      if (cursor.movePosition(QTextCursor::Right)) {
        ++inputCharCount;
        this->setTextCursor(cursor);
      } else {
        QApplication::beep();
      }
    } else if (key == Qt::Key_Tab) {
      // TODO: see if we can hack in tab completion.  See below for current status....
      //
      // do nothing on tabs.  Tab completion is actually being activated in the process
      // but it's apparently not being spat out using stdout.  For now, we don't output
      // anything in the QTextEdit when tab is pressed, but we do pass the character to
      // the bash process.
    } else {
      QString text = event->text();
      for (int i = 0; i < text.length(); ++i) {
        if (text.at(i).isPrint()) {
          //only increase input counter for printable characters
          ++inputCharCount;
        }
      }
      QTextEdit::keyPressEvent(event);
    }
  } else {
    if (inputCharCount) {
      --inputCharCount;
      QTextEdit::keyPressEvent(event);
    } else {
      QApplication::beep();
    }
  }

  // now pass a char* copy of the input to the shell process
  shell->write(event->text().toAscii().data(), event->text().length());
  curCursorLoc = this->textCursor();
}
