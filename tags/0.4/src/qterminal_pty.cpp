#include <cstdlib>
#include <sys/errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif
#include <QtGui>
#include <QtDebug>
#include "qterminal_pty.h"
#include "utils.h"

QTerminal::QTerminal(QWidget *parent, Qt::WindowFlags f) : QTextEdit(parent) {
  setWindowFlags(f);
  qDebug() << "Constructing QTerminal";
  savedCursor = this->textCursor();

  /*
   * Launch a shell connected to the current process by a PTY
   */
  shellPid = forkpty(&fdMaster, NULL, NULL, NULL);
  if (0 == shellPid) { /* fork off a shell */
    /* if they don't have a shell, use bash */
    setenv("SHELL", "/bin/bash", 0);
    /* no special control features are supported */
    setenv("TERM", "xterm", 1);
    /* hand off to the shell of choice */
    execl(getenv("SHELL"), getenv("SHELL"), (char *)0);
  }
  
  /*
   * We will use non-blocking I/O to read from the shell
   */
  fcntl(fdMaster, F_SETFL, fcntl(fdMaster, F_GETFL, 0) | O_NONBLOCK);  
  qDebug() << "Shell PID " << shellPid << " on file descriptor " << fdMaster;

  
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
  int kidstatus;
  wait(&kidstatus);
}

void QTerminal::readOutput() {
  this->setTextCursor(savedCursor);
  char c;
  int count = read(fdMaster, &c, 1);
  while(count == 1) {
    // ensure that the QTextEdit cursor keeps up with the terminal
    this->moveCursor(QTextCursor::NoMove);
    if (c == '\x1b') { /* escape character */
      handleEscape();
    } else if (c == '\r') {
      /* ignore */
  	} else if (c == '\x08') { /* ^H (backspace) */
  	  this->moveCursor(QTextCursor::PreviousCharacter);
  	} else if (c == '\a') {
  	  QApplication::beep();
  	} else {
  	  if (!textCursor().atEnd()) {
  	    textCursor().deleteChar();
  	  }
  	  this->insertPlainText(QChar(c));
  	}
    count = read(fdMaster, &c, 1);
  }
  if (count == -1 && errno == EAGAIN) {
    /* no data ready; no action necessary */
  } else {
    emit shellExited();
  }
  savedCursor = this->textCursor();
}

void QTerminal::handleEscape() {
  char nextChar = 0;
  read(fdMaster, &nextChar, 1);
  switch (nextChar) {
    case '[': //control sequence introducer
      handleControlSeq();
    break;
    
    case ']': //operating system command
      handleOSCommand();
    break;
  }
}

void QTerminal::handleControlSeq() {
  char nextChar = 0;
  QString seq = "";
  read(fdMaster, &nextChar, 1);
  
  while (!isCsiTerminator(nextChar)) {
    seq += nextChar;
    read(fdMaster, &nextChar, 1);
  }

  switch (nextChar) {
    case 'C': //move cursor forward one character
      this->moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
    case 'J':
      eraseDisplay(seq.toInt());
    break;
    
    case 'K':
      eraseInLine(seq.toInt());
    break;
    
    case 'P':
      deleteChars(seq.toInt());
  }
}

bool QTerminal::isCsiTerminator(char c) {
  QString chars = "@ABCDEFGHIJKLMPSTXZ`bcdfghilmnpqrstuvwxz{|";
  QChar theChar = QChar(c);
  // make the compiler happy
  for (int i = 0; i < chars.length(); ++i) {
    if (chars.at(i) == theChar) {
      return true;
    }
  }
  return false;
}

void QTerminal::handleOSCommand() {
  char nextChar = 0;
  char otherChar = 0;
  QString cmd = "";
  read(fdMaster, &nextChar, 1);
  
  while (!isOscTerminator(nextChar, otherChar)) {
    cmd += nextChar;
    
    if (otherChar == 0) {
      read(fdMaster, &nextChar, 1);
    } else {
      nextChar = otherChar;
      otherChar = 0;
    }
  }
  
  cmd += nextChar;
  
  switch(nextChar) {
    case '\x07': //ascii BEL character
      if (cmd.at(0) == '1') {
        QPalette palette = this->palette();
        QString colorStr = cmd;
        colorStr.remove(0,3);
        colorStr.remove(colorStr.length() - 1, 1);
        
        if (cmd.at(1) == '0') {
          qDebug() << "setting terminal text color to " << colorStr;

          if (colorStr.toLower() == "green") {
            palette.setColor(QPalette::Text, QColor((QRgb)0x00FF00));
          } else {
            palette.setColor(QPalette::Text, QColor(colorStr));
          }
          
        } else if (cmd.at(1) == '1') {
          qDebug() << "setting terminal background color to " << colorStr;
          palette.setColor(QPalette::Base, QColor(colorStr));
        }
        
        this->setPalette(palette);
      }
    break;
  }
}

bool QTerminal::isOscTerminator(char c, char & d) {
  if (c == '\x1b') {
    read(fdMaster, &d, 1);
    if (d == '\\') {
      return true;
    } else {
      return false;
    }
  } else if (c == '\x07'){
    return true;
  }
  
  return false;
}

void QTerminal::keyPressEvent(QKeyEvent * event) {  
  // pass a char* copy of the input to the shell process
  //shell->write(event->text().toAscii().data(), event->text().length());
  if (event->key() == Qt::Key_Up) {
    write(fdMaster, "\x1b\x5b\x41", 3);
    event->accept();
  } else if (event->key() == Qt::Key_Down) {
    write(fdMaster, "\x1b\x5b\x42", 3);
    event->accept();
    // FIXME: make sure left and right arrows work
  } else if (event->key() == Qt::Key_Right) {
    write(fdMaster, "\x1b\x5b\x43", 3);
    //this->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
    event->accept();
  } else if (event->key() == Qt::Key_Left) {
    write(fdMaster, "\x1b\x5b\x44", 3);
    //this->moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
    event->accept();
  } else {
    write(fdMaster, event->text().toAscii().data(), event->text().length());
    event->accept();
  }
}

void QTerminal::eraseDisplay(int arg) {
  switch (arg) {
    case 0:
    break;
    case 1:
    break;
    case 2:
      this->clear();
    break;
    case 3:
    break;
  }
}

void QTerminal::eraseInLine(int arg) {
  switch (arg) {
    case 0:
      this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
      this->textCursor().deleteChar();
    break;
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
  }
}

void QTerminal::deleteChars(int arg) {
  for (int i = 0; i < arg; ++i) {
    this->textCursor().deleteChar();
  }
}

void QTerminal::insertFromMimeData(const QMimeData* data) {
	QString s = data->text();
	write(fdMaster, s.toAscii().data(), s.length());
}

void QTerminal::changeDir(const QString & dir) {
	/* Hope we are talking to a prompt and not a text editor */
	write(fdMaster, "cd ", 3);
	write(fdMaster, dir.toAscii().data(), dir.length());
	write(fdMaster, "\n", 1);
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
		 	//qDebug() << "readyForRead()";
			emit readyForRead(watchedFd);
		} else {
			//qDebug() << "select() timeout";
		}
	}
	
	qDebug() << "FileDescriptorMonitor exiting";
}
	
void FileDescriptorMonitor::stop() {
	shouldStop = true;
}
