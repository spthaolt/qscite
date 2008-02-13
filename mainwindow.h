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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QAction;
class QMenu;
class QsciScintilla;
class QsciLexerBash;
class QsciLexerCPP;
class QsciLexerCSharp;
class QsciLexerCSS;
class QsciLexerHTML;
class QsciLexerJava;
class QsciLexerPerl;
class QsciLexerPython;
class QsciLexerRuby;
class QTabWidget;
//class QVector<QsciScintilla *>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    bool closeFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void curDocChanged();
    void setDocumentModified(bool wasModified = true);
    
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void redoSetMargin();
    void setLexerType(const QString & fileName);
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
//    QString strippedName(const QString &fullFileName);
    void createDocument();
    void changeTabs(int index);
    void documentWasModified();
    

    QsciScintilla * curDoc;
    std::vector<QsciScintilla *> * openFiles;
    QTabWidget * tabWidget;
    std::vector<QString> * fileNames;

    // currently, we have to have a different pointer for each lexer type...
    QsciLexerBash * lexer_bash;
    QsciLexerCPP * lexer_cpp;
    QsciLexerCSharp * lexer_csharp;
    QsciLexerCSS * lexer_css;
    QsciLexerHTML * lexer_html;
    QsciLexerJava * lexer_java;
    QsciLexerPerl * lexer_perl;
    QsciLexerPython * lexer_python;
    QsciLexerRuby * lexer_ruby;
    
    
    QString curFile;
    std::vector<bool> modified;

    QMenu *fileMenu;
    QMenu *editMenu;
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
    QAction *aboutAct;
    QAction *aboutQtAct;
};

#endif
