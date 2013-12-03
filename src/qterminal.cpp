#include <QtWidgets>
#include "qterminal.h"

QTerminal::QTerminal(QWidget *parent, Qt::WindowFlags f) : QTextEdit(parent) {
  setWindowFlags(f);
  cmdStr = "";
  shell = new QProcess();
  shell->setProcessChannelMode(QProcess::MergedChannels);
  QObject::connect(shell, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOut()));
  QObject::connect(shell, SIGNAL(readyReadStandardError()), this, SLOT(readStandardErr()));
  shell->start("cmd.exe", QStringList() << "", QIODevice::ReadWrite);
  //this->insertPlainText("TERMINAL VERY ALPHA QUALITY ON WIN32!\r\n");
  // This var protects against mouse interference with the cursor
  curCursorLoc = this->textCursor();
  inputCharCount = 0;
  histLocation = -1;
  tempCmd = "";
}

QTerminal::~QTerminal() {
  shell->kill();
  shell->waitForFinished();
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

void QTerminal::changeDir(const QString & dir) {
	/* Hope we are talking to a prompt and not a text editor */
  QString theDir = QString(dir);
  theDir = theDir.replace(QChar('/'), "\\");
	shell->write("cd ", 3);
	shell->write(theDir.toAscii().data(), theDir.length());
	shell->write("\r\n", 2);
}

void QTerminal::keyPressEvent(QKeyEvent * event) {
  int key = event->key();
  
  this->setTextCursor(curCursorLoc);
  
  // Keep QTextEdit in sync with shell as much as possible...
  if (key != Qt::Key_Backspace) {
    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
      inputCharCount = 0;
    } else if (key == Qt::Key_Up) {
      if (cmdHistory.size()) {
        if (histLocation == -1) {
          histLocation = cmdHistory.size() - 1;
          tempCmd = cmdStr;
        } else if (histLocation == 0) {
          QApplication::beep();
          event->ignore();
          return;
        } else {
          --histLocation;
        }
        
        for (int i = 0; i < inputCharCount; ++i) {
          QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
        }
        
        inputCharCount = cmdHistory.at(histLocation).length();
        this->insertPlainText(cmdHistory.at(histLocation));
        cmdStr = cmdHistory.at(histLocation);
      }
      
      event->ignore();
      return;
    } else if (key == Qt::Key_Down) {
      QString str = "";
      if (histLocation == -1) {
        QApplication::beep();
        event->ignore();
        return;
      } else if (histLocation == cmdHistory.size() - 1) {
        histLocation = -1;
        str = tempCmd;
      } else {
        ++histLocation;
        str = cmdHistory.at(histLocation);
      }
      
      for (int i = 0; i < inputCharCount; ++i) {
        QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
      }
      
      inputCharCount = str.length();
      this->insertPlainText(str);
      cmdStr = str;
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
      // TODO: see if we can hack in tab completion.  Currently we don't do anything.
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
      cmdStr.remove(inputCharCount, 1);
    } else {
      QApplication::beep();
    }
  }

  // now pass a char* copy of the input to the shell process
  if (key == Qt::Key_Return || key == Qt::Key_Enter) {
    this->moveCursor(QTextCursor::End);
    shell->write(cmdStr.toAscii().data(), cmdStr.length());
    shell->write("\r\n", 2);
    QTextEdit::keyPressEvent(event);
    cmdHistory.push_back(cmdStr);
    histLocation = -1;
    cmdStr = "";
    tempCmd = "";
  } else {
    QString input = event->text();
    for (int i = 0; i < input.length(); ++i) {
      if (input.at(i).isPrint()) {
        cmdStr.insert(inputCharCount - 1, input.at(i));
      }
    }
  }
  curCursorLoc = this->textCursor();
}
