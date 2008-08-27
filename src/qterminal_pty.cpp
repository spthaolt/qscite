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
// #include "utils.h"

QTerminal::QTerminal(QWidget *parent, Qt::WindowFlags f) : QTextEdit(parent) {
  setWindowFlags(f);
  qDebug() << "Constructing QTerminal";
  savedCursor = this->textCursor();
  sequenceState = NoSequence;

  // The Following line causes the tab key to change focus...
  //setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
  //

  setTabChangesFocus(false);
  setFocusPolicy(Qt::ClickFocus);

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
    /* If this exec returns, we should take some action.
     * Window systems do ugly things if two processes try
     * to use the same resources. */
    if (errno == ENOENT) {
      execl("/bin/sh", "/bin/sh", (char*)0);
    }
    perror("QTerminal: exec");
     // exit hard in case exit handlers mess with the window system
     _exit(127);
  } else if (-1 == shellPid) { // didn't fork
    this->setText(QString("QTerminal: fork: %1\n").arg(strerror(errno)));
  } else {

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
}

QTerminal::~QTerminal() {
  watcher->stop();
  watcher->disconnect();
  if (shellPid > 0) {
    kill(shellPid, SIGHUP);
  }
  watcher->wait();
  ::close(fdMaster);
  int kidstatus;
  if (shellPid > 0) {
    waitpid(shellPid, &kidstatus, 0);
  }
}

void QTerminal::readOutput() {
  this->setTextCursor(savedCursor);
  
  char rdBuf[256];
  ssize_t count = read(fdMaster, rdBuf, sizeof(rdBuf)/sizeof(char));
  
  if (0 == count || (-1 == count && errno == EIO)) { // EOF, child closed
    watcher->stop();
    watcher->disconnect();
    int childStatus;
    pid_t waitedPid = waitpid(shellPid, &childStatus, 0);
    if (waitedPid == shellPid) {
      shellPid = 0;
      if (WIFEXITED(childStatus)) {
        if(WEXITSTATUS(childStatus) == 0) {
          emit shellExited();
        } else {
          this->insertPlainText(QString("%1 exited with status %2.\n").arg(waitedPid).arg(WEXITSTATUS(childStatus)));
        }
      } else if (WIFSIGNALED(childStatus)) {
        this->insertPlainText(QString("%1 exited with signal %2.\n").arg(waitedPid).arg(WTERMSIG(childStatus)));
      } else {
        this->insertPlainText("QTerminal: Why are WIFEXITED and WIFSIGNALED both false?\n");
      }
      return;
    } else {
      perror("QTerminal: waitpid");
    }
  }
  if ((-1 == count) && (errno != EAGAIN)) {
    perror("QTerminal: read");
    return;
  }

  for (char * c = rdBuf; c < rdBuf + count; ++c) {
    switch (sequenceState) {
      case GotEsc:
        if (*c == '[') {
          sequenceState = InCS;
        } else if (*c == ']') {
          sequenceState = InOSC;
        } else {
          qWarning("No Can Do ESC %c", *c);
          sequenceState = NoSequence;
        }
        break;
      case InCS:
        if (*c >= '\x20' && *c <= '\x7e') {
          savedSequence += *c;
          if (*c >= '\x40') {
            doControlSeq(savedSequence);
            savedSequence.clear();
            sequenceState = NoSequence;
          }
        } else {
          /* ANSI says control or high-half bytes shouldn't happen
           * inside a control sequence. ANSI didn't write every program
           * in existence.
           */
          qWarning("No Can Do Esc [ %s", savedSequence.toHex().constData());
          savedSequence.clear();
          sequenceState = NoSequence;
        }
        break;
      case InOSC:
        savedSequence += *c;
        if (*c  == '\x07') {
          doOSCommand(savedSequence);
          savedSequence.clear();
          sequenceState = NoSequence;
        } else if (*c == '\x1b') {
          sequenceState = OSCHalfClosed;
        }
        break;
      case OSCHalfClosed:
        savedSequence += *c;
        if (*c == '\\') {
          doOSCommand(savedSequence);
        } else {
          qWarning("No Can Do Esc ] %s", savedSequence.toHex().constData());
        }
        savedSequence.clear();
        sequenceState = NoSequence;
        break;
      default:
        qWarning("QTerminal::sequenceState got set to %d", sequenceState);
        sequenceState = NoSequence;
        // FALL THROUGH
      case NoSequence:
        switch (*c) {
          case '\x1b':
            /* ASCII ESC. */
            sequenceState = GotEsc;
            break;
          case '\r':
            /* Ignore CR.
             * We assume that CR will only be sent as part of a CR/LF pair,
             * and in that case processing the LF does all we need.
             */
            break;
          case '\x08':
            /* Backspace. We assume non-destructive. */
            this->moveCursor(QTextCursor::PreviousCharacter);
            break;
          case '\a':
            /* ASCII BEL. */
            QApplication::beep();
            break;
          case '\n':
            /* Each insertPlainText() we do causes a paint event.
             * We processEvents() at the end of each line so those
             * paint events can be processed and the user can see
             * something happen.
             */
            qApp->processEvents();
            // FALL THROUGH
          default:
            if (!(this->textCursor().atEnd())) {
              this->textCursor().deleteChar();
            }
            this->insertPlainText(QChar(*c));
            break;
        }
        break;
    }
  }
  savedCursor = this->textCursor();
}

void QTerminal::doControlSeq(const QByteArray & seq) {
  switch (seq.right(1).at(0)) {
    case 'C':
      if (this->textCursor().atEnd()) {
        this->insertPlainText(" ");
      } else {
        this->moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
      }
      break;
    case 'J':
      eraseDisplay(seq.left(seq.length() - 1).toInt());
      /* Perl would parse the number at the beginning of the string :-P */
      break;
    case 'K':
      eraseInLine(seq.left(seq.length() - 1).toInt());
      break;
    case 'P':
      deleteChars(seq.left(seq.length() - 1).toInt());
      break;
    default:
      qDebug("No Can Do ESC [ %s", seq.constData());
      break;
  }
}

void QTerminal::doOSCommand(const QByteArray & cmd) {
  int termLength = (cmd.endsWith("\x1b\\") ? 2 : 1);
  int firstParam = cmd.left(cmd.indexOf(';')).toInt();
  QByteArray textParam = cmd.left(cmd.length() - termLength).mid(cmd.indexOf(';') + 1);
  QPalette activePalette = this->palette();
  /* TODO: XTerm technically allows a series of ;-separated color specifications,
   * with subsequent colors being assigned to subsequent slots.
   * The old code didn't try, and this doesn't try yet.
   */
  switch (firstParam) {
    case 10:
      /* Qt displays "green" as #008000. Anyone using it as a text color
       * probably has a dark background and wants the traditional X11 #00FF00.
       */
      activePalette.setColor(QPalette::Text,
          (textParam.toLower() == "green") ? QColor(0x00FF00) : QColor(textParam.constData()));
      this->setPalette(activePalette);
      break;
    case 11:
      activePalette.setColor(QPalette::Text, QColor(textParam.constData()));
      this->setPalette(activePalette);
      break;
    default:
      qDebug("No Can Do ESC ] %s", cmd.toHex().constData());
      break;
  }
}

void QTerminal::keyPressEvent(QKeyEvent * event) {
  // pass a char* copy of the input to the shell process
  //shell->write(event->text().toAscii().data(), event->text().length());
  if (event->key() == Qt::Key_Up) {
    write(fdMaster, "\x1b\x5b\x41", 3);
  } else if (event->key() == Qt::Key_Down) {
    write(fdMaster, "\x1b\x5b\x42", 3);
    // FIXME: make sure left and right arrows work
  } else if (event->key() == Qt::Key_Right) {
    write(fdMaster, "\x1b\x5b\x43", 3);
    //this->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);
  } else if (event->key() == Qt::Key_Left) {
    write(fdMaster, "\x1b\x5b\x44", 3);
    //this->moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
  } else {
    write(fdMaster, event->text().toAscii().data(), event->text().length());
  }

  event->accept();
}

void QTerminal::keyReleaseEvent(QKeyEvent * event) {
  event->accept();
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
  /* Ben: Hope we are talking to a prompt and not a text editor */
  /* Jared: Well, since our terminal probably can't support much more
   *        than a simple line editor anyway... */
  /* FIXME: When we start the terminal, we need to make sure to record the PID...
   *        then when we get to the current section of code, we need to check
   *        the process table to see if there are any processes running with the
   *        PPID matching our recorded PPID.   Or at least that should prove
   *        effective in most cases (i.e., the user hasn't backgrounded a child
   *        process).
   */
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

/* vim: set softtabstop=2 shiftwidth=2 expandtab : */
