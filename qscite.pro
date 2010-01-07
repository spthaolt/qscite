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

CONFIG       += thread

HEADERS       = mainwindow.h \
                utils.h \
                lexer_utils.h \
                prefs.h \
                textdisplay.h \
                findreplace.h \
                launcher.h

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
                launcher.cpp

FORMS         = dlgPrefsUI.ui \
                frmTextDisplay.ui \
                dlgConvertIndent.ui \
                dlgFindReplace.ui

RESOURCES     = qscite.qrc

unix {
  HEADERS += qterminal_pty.h
  SOURCES += qterminal_pty.cpp
  
  !macx {
    LIBS    += -lqscintilla2 \
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
