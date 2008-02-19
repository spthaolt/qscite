#ifdef QSCITE_DEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <sys/errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif
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
  if (0 == shellPid) { /* fork off a shell */
    /* if they don't have a shell, use bash */
    setenv("SHELL", "/bin/bash", 0);
    /* no special control features are supported */
    setenv("TERM", "dumb", 1);
    /* hand off to the shell of choice */
    execl(getenv("SHELL"), getenv("SHELL"), (char *)0);
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
  connect(this, SIGNAL(shellExited()), watcher, SLOT(stop()));
  watcher->start();
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
  	if (c == '\r') {
      /* ignore */
  	} else if (c == '\x08') { /* ^H (backspace) */
  	  textCursor().deletePreviousChar();
  	} else if (c == '\a') {
  	  QApplication::beep();
  	} else {
	  this->insertPlainText(QChar(c));
	  this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
	}
    count = read(fdMaster, &c, 1);
  }
  if (count == -1 && errno == EAGAIN) {
    /* no data ready; no action necessary */
  } else {
    emit shellExited();
  }
}

void QTerminal::keyPressEvent(QKeyEvent * event) {  
  // pass a char* copy of the input to the shell process
  //shell->write(event->text().toAscii().data(), event->text().length());
  write(fdMaster, event->text().toAscii().data(), event->text().length());
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
		workingFdSet = watchedFdSet;
		int rval = select(watchedFd + 1, &workingFdSet, NULL, NULL, &workingPollInterval);
		workingPollInterval = pollInterval;
		if (rval != -1 && FD_ISSET(watchedFd, &workingFdSet)) {
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
