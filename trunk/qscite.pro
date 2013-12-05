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

unix {
  qsci_project.target = QScintilla2/Qt4Qt5/qscintilla.pro
  qsci_project.commands = ./get_qsci.sh

  qsci_lib.target = $$OBJECTS_DIR/libqscintilla2.a
  qsci_lib.commands  = cd QScintilla2/Qt4Qt5 &&
  qsci_lib.commands += sed -i \"s/dll/static/\" qscintilla.pro &&
  qsci_lib.commands += $(QMAKE) qscintilla.pro &&
  qsci_lib.commands += $(MAKE) -j `cat /proc/cpuinfo | grep '^processor' | wc -l` &&
  qsci_lib.commands += cd ../.. && cp QScintilla2/Qt4Qt5/libqscintilla2.a $(OBJECTS_DIR)
  qsci_lib.depends = qsci_project

  QMAKE_EXTRA_TARGETS += qsci_project qsci_lib
  PRE_TARGETDEPS += $$qsci_lib.target
  QMAKE_CLEAN += $$qsci_lib.target QScintilla2
}

QT += script\
      widgets

CONFIG       += thread

HEADERS       = include/mainwindow.h \
                include/utils.h \
                include/lexer_utils.h \
                include/prefs.h \
                include/textdisplay.h \
                include/findreplace.h \
                include/scriptconsole.h \
                include/launcher.h \
                include/qsciteeditor.h \

SOURCES       = src/main.cpp \
                src/mainwindow.cpp \
                src/mainwindow_startup.cpp \
                src/mainwindow_uislots.cpp \
                src/mainwindow_prefs.cpp \
                src/utils.cpp \
                src/lexer_utils.cpp \
                src/prefs.cpp \
                src/textdisplay.cpp \
                src/reindent.cpp \
                src/findreplace.cpp \
                src/scriptconsole.cpp \
                src/launcher.cpp \
                src/qsciteeditor.cpp

FORMS         = ui/dlgPrefsUI.ui \
                ui/frmTextDisplay.ui \
                ui/dlgConvertIndent.ui \
                ui/dlgFindReplace.ui \
                ui/dlgScriptConsole.ui

RESOURCES     = qscite.qrc

unix {
  HEADERS += include/qterminal_pty.h
  SOURCES += src/qterminal_pty.cpp
  INCLUDEPATH += QScintilla2/Qt4Qt5
  LIBS    += build/libqscintilla2.a
  !macx {
    LIBS    += -lutil
    DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
    TARGET = qscite
    target.path = $$PREFIX/bin/
    desktopfile.files = qscite.desktop
    desktopfile.path = $$PREFIX/share/applications/
    INSTALLS += target desktopfile
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
