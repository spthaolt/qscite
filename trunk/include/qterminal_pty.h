#ifndef QTERMINAL_H_
#define QTERMINAL_H_
#include <QTextEdit>
#include <QThread>
#include <sys/select.h>

class FileDescriptorMonitor;

class QTerminal : public QTextEdit {
	Q_OBJECT
public:
	QTerminal(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~QTerminal();
	void keyPressEvent(QKeyEvent * event);

signals:
	void shellExited();

private slots:
	void readOutput();
private:
	int shellPid;
	int fdMaster;
	FileDescriptorMonitor * watcher;
	// We need to know where our original
	QTextCursor savedCursor;
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
