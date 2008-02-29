#include <QtGui>
#include "qterminal.h"

QTerminal::QTerminal(QWidget *parent, Qt::WindowFlags f) : QTextEdit(parent) {
  setWindowFlags(f);
  cmdStr = "";
  shell = new QProcess();
  shell->setProcessChannelMode(QProcess::MergedChannels);
  QObject::connect(shell, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOut()));
  QObject::connect(shell, SIGNAL(readyReadStandardError()), this, SLOT(readStandardErr()));
  //QObject::connect(shell, SIGNAL(finished()), this, SLOT(processFinished()));
  shell->start("cmd.exe", QStringList() << "/C" << "cmd.exe", QIODevice::ReadWrite);
  this->insertPlainText("TERMINAL VERY ALPHA QUALITY ON WIN32!\r\n");
  // This var protects against mouse interference with the cursor
  curCursorLoc = this->textCursor();
  inputCharCount = 0;
}

QTerminal::~QTerminal() {
  shell->kill();
  shell->waitForFinished();
  delete shell;
}

void QTerminal::printPrompt () {
  this->insertPlainText(QApplication::applicationDirPath() + ">");
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

void QTerminal::processFinished() {
  printPrompt();
}

void QTerminal::changeDir(const QString & dir) {
	/* Hope we are talking to a prompt and not a text editor */
	shell->write("cd ", 3);
	shell->write(dir.toAscii().data(), dir.length());
	shell->write("\r\n", 2);
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
  if (key == Qt::Key_Return || key == Qt::Key_Enter) {
    shell->write("\r\n", 2);
  } else if (key == Qt::Key_Backspace) {
    shell->write("\0x1AH", 2);
  } else {
    shell->write(event->text().toAscii().data(), event->text().length());
  }
  curCursorLoc = this->textCursor();
}
