TEMPLATE     = app

DEPENDPATH   += include src ui

INCLUDEPATH  += include

MOC_DIR     = build
OBJECTS_DIR = build
RCC_DIR     = build
UI_DIR      = build
!macx {
    DESTDIR = build
}

qsci_project.target = QScintilla2/Qt4/qscintilla.pro
qsci_project.commands = ./get_qsci.sh

qsci_lib.target = $$OBJECTS_DIR/libqscintilla2.a
qsci_lib.commands  = cd QScintilla2/Qt4 &&
qsci_lib.commands += $(QMAKE) -after \"CONFIG += static\" qscintilla.pro &&
qsci_lib.commands += $(MAKE) &&
qsci_lib.commands += cd ../.. && cp QScintilla2/Qt4/libqscintilla2.a $(OBJECTS_DIR)
qsci_lib.depends = qsci_project

QMAKE_EXTRA_TARGETS += qsci_project qsci_lib
PRE_TARGETDEPS += $$qsci_lib.target
QMAKE_CLEAN += $$qsci_lib.target QScintilla2

QT += script

CONFIG       += thread

HEADERS       = mainwindow.h \
                utils.h \
                lexer_utils.h \
                prefs.h \
                textdisplay.h \
                findreplace.h \
                scriptconsole.h \
                launcher.h \
                qsciteeditor.h \

SOURCES       = main.cpp \
                mainwindow.cpp \
                mainwindow_startup.cpp \
                mainwindow_uislots.cpp \
                mainwindow_prefs.cpp \
                utils.cpp \
                lexer_utils.cpp \
                prefs.cpp \
                textdisplay.cpp \
                reindent.cpp \
                findreplace.cpp \
                scriptconsole.cpp \
                launcher.cpp \
                qsciteeditor.cpp

FORMS         = dlgPrefsUI.ui \
                frmTextDisplay.ui \
                dlgConvertIndent.ui \
                dlgFindReplace.ui \
                dlgScriptConsole.ui

RESOURCES     = qscite.qrc

unix {
  HEADERS += qterminal_pty.h
  SOURCES += qterminal_pty.cpp
  INCLUDEPATH += QScintilla2/Qt4
  LIBS    += build/libqscintilla2.a
  !macx {
    LIBS    += -lutil
    DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
  } else {
    TARGET   = QSciTE
    DEFINES += QSCITE_MONO_FAMILY='\\"Monaco\\"'
  }
}

win32 {
  LIBS    += -lqscintilla2
  DEFINES += QSCITE_MONO_FAMILY='"\\"Courier New\\""'
  HEADERS += qterminal.h
  SOURCES += qterminal.cpp
}

release {
  DEFINES += QT_NO_DEBUG_OUTPUT
}
