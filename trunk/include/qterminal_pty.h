#ifndef QTERMINAL_H_
#define QTERMINAL_H_
#include <QTextEdit>
#include <QThread>
#include <sys/select.h>
class QMimeData;

class FileDescriptorMonitor;

class QTerminal : public QTextEdit {
	Q_OBJECT
public:
	QTerminal(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~QTerminal();

public slots:
	void changeDir(const QString & dir);

signals:
	void shellExited();

protected:
	void insertFromMimeData(const QMimeData * data);
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);

private slots:
	void readOutput();
private:
	int shellPid;
	int fdMaster;
	FileDescriptorMonitor * watcher;
	// We need to know where our original
	QTextCursor savedCursor;
	bool isCsiTerminator(char c);
	bool isOscTerminator(char c, char & d);
	void handleEscape();
	void handleControlSeq();
	void handleOSCommand();
	void eraseDisplay(int arg);
	void eraseInLine(int arg = 0);
	void deleteChars(int arg = 1);
};

class FileDescriptorMonitor: public QThread {
	Q_OBJECT
public:
	FileDescriptorMonitor(int fd, QObject * parent = 0);
public slots:
	void stop();
signals:
	void readyForRead(int fd);
protected:
	void run();
private:
	int watchedFd;
	fd_set watchedFdSet;
	bool shouldStop;
};

#endif /*QTERMINAL_H_*/
