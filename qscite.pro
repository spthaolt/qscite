TEMPLATE     = app

DEPENDPATH   += include src ui

INCLUDEPATH  += . include

MOC_DIR     = build
OBJECTS_DIR = build
RCC_DIR     = build
UI_DIR      = build

CONFIG       += release thread

HEADERS       = mainwindow.h \
                utils.h \
                lexer_utils.h \
                prefs.h

SOURCES       = main.cpp \
                mainwindow.cpp \
                mainwindow_startup.cpp \
                utils.cpp \
                lexer_utils.cpp \
                prefs.cpp 

FORMS         = dlgPrefsUI.ui

RESOURCES     = qscite.qrc

unix {
	!macx {
		LIBS         += -lqscintilla2 \
					    -lutil
		DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
	}
  HEADERS += qterminal_pty.h
  SOURCES += qterminal_pty.cpp
}

macx {
	TARGET = QSciTE
	LIBS += -framework qscintilla2
	DEFINES += QSCITE_MONO_FAMILY='\\"Monaco\\"'
}

win32 {
	LIBS         += -lqscintilla2
	DEFINES += QSCITE_MONO_FAMILY='"\\"Courier New\\""'
  HEADERS += qterminal.h
  SOURCES += qterminal.cpp
}

debug {
	CONFIG += warn_on
	DEFINES += QSCITE_DEBUG
}
