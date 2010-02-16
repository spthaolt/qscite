TEMPLATE     = app

DEPENDPATH   += include src ui

INCLUDEPATH  += include \
                qtermwidget

MOC_DIR     = .moc
OBJECTS_DIR = build
RCC_DIR     = build
UI_DIR      = build
!macx {
    DESTDIR = build
}

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
  
  !macx {
    LIBS    += -L./build \
               -lqtermwidget \
               -lqscintilla2 \
               -lutil
    DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
  } else {
    TARGET   = QSciTE
    LIBS    += -framework qscintilla2
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
