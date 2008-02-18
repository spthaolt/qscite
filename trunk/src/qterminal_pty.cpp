#ifdef QSCITE_DEBUG
#include <iostream>
#endif

#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <util.h>

#include <QtGui>
#include "qterminal_pty.h"

QTerminal::QTerminal(QWidget *parent, Qt::WindowFlags f) : QTextEdit(parent) {
  setWindowFlags(f);
#ifdef QSCITE_DEBUG
  std::cout << "Constructing QTerminal" << std::endl;
#endif
  /*
   * Launch a shell connected to the current process by a PTY
   */
  shellPid = forkpty(&fdMaster, NULL, NULL, NULL);
  if (0 == shellPid) {
    execlp("bash", "bash", "-i", (char *)0);
  }
  
  /*
   * We will use non-blocking I/O to read from the shell
   */
  fcntl(fdMaster, F_SETFL, fcntl(fdMaster, F_GETFL, 0) | O_NONBLOCK);

#ifdef QSCITE_DEBUG  
  std::cout << "Shell PID " << shellPid << " on file descriptor " << fdMaster << std::endl;
#endif
  
  /*
   * Arrange to be notified when the shell emits output
   */
  watcher = new FileDescriptorMonitor(fdMaster, this);
  connect(watcher, SIGNAL(readyForRead(int)), this, SLOT(readOutput()), Qt::BlockingQueuedConnection);
  watcher->start();
  
  // This var protects against mouse interference with the cursor
  curCursorLoc = this->textCursor();
  inputCharCount = 0;
}

QTerminal::~QTerminal() {
  watcher->stop();
  watcher->disconnect();
  kill(shellPid, SIGHUP);
  watcher->wait();
  ::close(fdMaster);
}

void QTerminal::readOutput() {
  char c;
  int count = read(fdMaster, &c, 1);
  while(count == 1) {
  	if (c != '\r') {
		this->insertPlainText(QChar(c));
		this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
	}
    count = read(fdMaster, &c, 1);
  }
}

void QTerminal::keyPressEvent(QKeyEvent * event) {
  int key = event->key();
  
  this->setTextCursor(curCursorLoc);
  
  // Keep QTextEdit in sync with shell as much as possible...
  if (key != Qt::Key_Backspace) {
    if (key == Qt::Key_Return || key == Qt::Key_Enter) {
      inputCharCount = 0;
      //QTextEdit::keyPressEvent(event);
    } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
      // TODO: see if there's a way to hack the bash history in here...
    } else if (key == Qt::Key_Left) {
      if (inputCharCount) {
        --inputCharCount;
        //QTextEdit::keyPressEvent(event);
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
      //QTextEdit::keyPressEvent(event);
    }
  } else {
    if (inputCharCount) {
      --inputCharCount;
      //QTextEdit::keyPressEvent(event);
    } else {
      QApplication::beep();
    }
  }

  // now pass a char* copy of the input to the shell process
  //shell->write(event->text().toAscii().data(), event->text().length());
  write(fdMaster, event->text().toAscii().data(), event->text().length());
  curCursorLoc = this->textCursor();
}

FileDescriptorMonitor::FileDescriptorMonitor(int fd, QObject * parent) :
	QThread(parent), watchedFd(fd)
{
	FD_ZERO(&watchedFdSet);
	FD_SET(watchedFd, &watchedFdSet);
}

void FileDescriptorMonitor::run() {
	shouldStop = false;
	fd_set workingFdSet;
	timeval pollInterval = {1, 0}; // 1 sec
	timeval workingPollInterval = pollInterval;
	while (!shouldStop) {
		FD_COPY(&watchedFdSet, &workingFdSet);
		select(watchedFd + 1, &workingFdSet, NULL, NULL, &pollInterval);
		workingPollInterval = pollInterval;
		if (FD_ISSET(watchedFd, &workingFdSet)) {
#ifdef QSCITE_DEBUG
		 	std::cout << "readyForRead()" << std::endl;
#endif
			emit readyForRead(watchedFd);
		}
#ifdef QSCITE_DEBUG
		else {
			std::cout << "select() timeout" << std::endl;
		}
#endif
	}
#ifdef QSCITE_DEBUG
	std::cout << "FileDescriptorMonitor exiting" << std::endl;
#endif
}
	
void FileDescriptorMonitor::stop() {
	shouldStop = true;
}
