/***************************************************************************
 *   Copyright (C) 2007-2008 by Jared Sutton <jpsutton@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _QSCITE_MAINWINDOW_H
#define _QSCITE_MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFileInfo>

class QAction;
class QMenu;
class QsciScintilla;
class QTabWidget;
class QTerminal;

struct FileData {

	FileData(QsciScintilla * doc = NULL);
	FileData(const FileData & src);
	
	QString fullName;
	QString baseName;
	QString path;
	
	QsciScintilla * edWidget;
	
	void setPathName(const QString & newPathName);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    /* Slots corresponding to UI actions */
    void newFile();
    bool closeFile();
    void open();
    void openRecent(QAction * src);
    bool save();
    bool saveAs();
    void editCopy();
    void editCut();
    void editPaste();
    void about();
    void fontDialog();
    void globalPrefs();
    void toggleTerminal(bool alive = false);
    
    /* Internal slots */
    void curDocChanged(int idx);
    void setDocumentModified(bool wasModified = true);
    void reapPrefs();
    void redoSetMargin();
    void prefsWereChanged();
    void updateCopyAvailable(bool);
    void noticeFocusChange(QWidget *, QWidget *);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setWindowTitleForFile(const QString & fileName);
    void setCurrentTabTitle();
    void createDocument();
    void changeTabs(int index);
    void documentWasModified();
    
    std::vector<FileData> openFiles;
    QList<QFileInfo> recentFiles;
    QTabWidget * tabWidget;
    QTerminal * termWidget;
    
    bool copyFromTerm;
    bool termInDrawer;

    unsigned int curDocIdx;

    QMenu *fileMenu;
    QMenu *recentMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *closeAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    
    QAction *prefsAct;
    
    QAction *aboutAct;
    QAction *aboutQtAct;
    
    QAction *fontAct;
    QAction * terminalAct;
};

#endif
