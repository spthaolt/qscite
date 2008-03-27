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
#include <QSystemTrayIcon>

#include "launcher.h"

class QAction;
class QMenu;
class QsciScintilla;
class QTabWidget;
class QTerminal;
class TextDisplayPanel;
class QStringList;
class QActionGroup;
class dlgFindText;

struct FileData {

	FileData(QsciScintilla * doc = NULL);
	FileData(const FileData & src);
	
	QString fullName;
	QString baseName;
	QString path;
	
	QsciScintilla * edWidget;
	
	void setPathName(const QString & newPathName);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

	friend class TextDisplayPanel;

public:
    MainWindow(QStringList & _argv, Launcher * _launcher);

    inline QsciScintilla * getCurDoc() {
      // this implementation is in the header file to avoid excessive warnings...
      // and to allow QSciTE to compile on Win32.
      return (openFiles.size() ? openFiles[curDocIdx].edWidget : NULL);
    }
    
    bool closeWindow();

protected:
    void closeEvent(QCloseEvent * event);
    bool eventFilter(QObject * target, QEvent * event);

private slots:
    /* Slots corresponding to UI actions */
    void newFile();
    bool closeFile();
    void open();
    void openRecent(QAction * src);
    bool save();
    bool saveAs();
    void undo();
    void redo();
    void editCopy();
    void editCut();
    void editPaste();
    void about();
    void fontDialog();
    void globalPrefs();
    void toggleTerminal(bool alive = false);
    void textDisplay();
    void prevDoc();
    void nextDoc();
    void setEolCr();
    void setEolLf();
    void setEolCrLf();
    void convertEols();
    void setEolVisibility(bool vis);
    void convertIndentation();
    void toggleFolding();
    void showFindDialog();
    void newWindow();
    
    /* Internal slots */
    void curDocChanged(int idx);
    void setDocumentModified(bool wasModified = true);
    void reapPrefs();
    void textDisplayDeleted();
    void redoSetMargin();
    void prefsWereChanged();
    void updateCopyAvailable(bool);
    void noticeFocusChange(QWidget *, QWidget *);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createTrayIcon();
    void createDocument();
    
    void readSettings();
    void writeSettings();
    
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void addRecentFile(const QString &fileName);
    
    void setWindowTitleForFile(const QString & fileName);
    void setCurrentTabTitle();
    void changeTabs(int index);
    void documentWasModified();
    void setUIForDocumentEolMode();
    
    QStringList argv;
    Launcher * launcher;
    std::vector<FileData> openFiles;
    QList<QFileInfo> recentFiles;
    QTabWidget * tabWidget;
    QTerminal * termWidget;
    TextDisplayPanel * textSettingsWidget;
    QMenu * trayIconMenu;
    QString lastDir;
    
    bool copyFromTerm;
    bool termInDrawer;

    unsigned int curDocIdx;

    QMenu * fileMenu;
    QMenu * recentMenu;
    QMenu * editMenu;
    QMenu * viewMenu;
    QMenu * optionsMenu;
    QMenu * lineEndMenu;
    QMenu * windowMenu;
    QMenu * helpMenu;
    QToolBar * mainToolBar;
    
    QAction * minimizeAction;
    QAction * maximizeAction;
    QAction * restoreAction;
    QAction * quitAction;
    QAction * newAct;
    QAction * closeAct;
    QAction * openAct;
    QAction * saveAct;
    QAction * saveAsAct;
    QAction * exitAct;
    QAction * undoAct;
    QAction * redoAct;
    QAction * cutAct;
    QAction * copyAct;
    QAction * pasteAct;
    
    QAction * prefsAct;
    
    QAction * aboutAct;
    QAction * aboutQtAct;
    
    QAction * fontAct;
    QAction * terminalAct;
    QAction * textDisplayAct;
    QAction * nextAct;
    QAction * prevAct;
    
    QAction * convertEndings;
    QAction * lineEndCr;
    QAction * lineEndLf;
    QAction * lineEndCrLf;
    QActionGroup * lineEnds;
    
    QAction * showLineEndsAct;
	
    QAction * convertIndentAct;
    QAction * codeFoldingAct;
    QAction * findTextAct;
    QAction * newWindowAct;
};

#endif
