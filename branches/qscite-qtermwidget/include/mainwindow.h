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
#include <QtScript>

#include "launcher.h"
#include "qsciteeditor.h"

class QAction;
class QMenu;
class QsciLexer;
class QTabWidget;
class QTerminal;
class QTermWidget;
class TextDisplayPanel;
class QStringList;
class QActionGroup;
class dlgFindReplace;
class dlgScriptConsole;

struct FileData {

  FileData(QsciteEditor * doc = NULL);
  FileData(const FileData & src);

  QString fullName;
  QString baseName;
  QString path;

  QsciteEditor * edWidget;

  void setPathName(const QString & newPathName);
};

class MainWindow : public QMainWindow {
  Q_OBJECT

  friend class TextDisplayPanel;

public:
  MainWindow(QStringList & _argv, Launcher * _launcher);

  inline FileData * getCurFileObj() {
    // this implementation is in the header file to avoid excessive warnings...
    // and to allow QSciTE to compile on Win32.
    return (openFiles.size() > 0 ? &openFiles[getCurDoc()] : NULL);
  }

  inline QsciteEditor * getCurDoc() {
    // this implementation is in the header file to avoid excessive warnings...
    // and to allow QSciTE to compile on Win32.
    return (openFiles.size() > 0 ? (QsciteEditor *)(tabWidget->currentWidget()) : NULL);
  }

  inline int getCurTabIndex() {
    return (openFiles.size() > 0 ? tabWidget->indexOf((QWidget *)getCurDoc()) : -1);
  }

  bool closeWindow();
  QScriptEngine scriptEngine;
  void dropEvent(QDropEvent * event);

  /*
  signals:
      void closed();
  */

protected:
  void closeEvent(QCloseEvent * event);
  bool eventFilter(QObject * target, QEvent * event);

private slots:
  /* Slots corresponding to UI actions */
  void newFile();
  bool closeFile();
  void open(QString fileName);
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
  void globalPrefs();
  void textDisplay();
  void prevDoc();
  void nextDoc();
  void setEolCr();
  void setEolLf();
  void setEolCrLf();
  void setEolVisibility(bool vis);
  void convertEols();
  void convertIndentation();
  void toggleTerminal(bool alive = false);
  void toggleFolding();
  void showFontDialog();
  void showFindDialog();
  void showReplaceDialog();
  void showScriptConsole();
  void newWindow();
  void closeTab(int tabIndex);

  /* Internal slots */
  void curDocChanged(int idx);
  void setDocumentModified(bool wasModified = true);
  void reapPrefs();
  void textDisplayDeleted();
  void redoSetMargin();
  void prefsWereChanged();
  void updateCopyAvailable(bool);
  void noticeFocusChange(QWidget *, QWidget *);
  void dragEnterEvent(QDragEnterEvent * event);
  void replaceDialogClosed();
  void scriptConsoleClosed();
  void lexerMenuChanged();

private:
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void createDocument();

  void readSettings();
  void writeSettings();

  bool maybeSave();
  void loadFile(const QString & fileName);
  bool saveFile(const QString & fileName);
  void addRecentFile(const QString & fileName);

  void setWindowTitleForFile(const QString & fileName);
  void setCurrentTabTitle();
  void changeTabs(QsciteEditor * edWidget);
  void changeTabs(int tabIndex);
  void documentWasModified();
  void setUIForDocumentEolMode();
  void setLexer(const QString & lexerName);
  void setLexer(QsciLexer * lexer);
  void setupDocument(QString & fileName);

  QStringList argv;
  Launcher * launcher;
  QMap<QsciteEditor *, FileData> openFiles;
  QList<QFileInfo> recentFiles;
  QTabWidget * tabWidget;
  QTermWidget * termWidget;
  TextDisplayPanel * textSettingsWidget;
  QMenu * trayIconMenu;
  QString lastDir;
  dlgFindReplace * replaceDialog;
  dlgScriptConsole * scriptConsole;

  bool copyFromTerm;
  bool termInDrawer;

  QMenu * fileMenu;
  QMenu * recentMenu;
  QMenu * editMenu;
  QMenu * viewMenu;
  QMenu * lexerMenu;
  QMenu * toolsMenu;
  QMenu * lineEndMenu;
  QMenu * windowMenu;
  QMenu * helpMenu;
  QToolBar * mainToolBar;

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
  QActionGroup * lexers;

  QAction * showLineEndsAct;

  QAction * convertIndentAct;
  QAction * codeFoldingAct;
  QAction * findTextAct;
  QAction * replaceTextAct;
  QAction * scriptConsoleAct;
  QAction * newWindowAct;
};

#endif