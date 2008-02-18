TEMPLATE     = app

DEPENDPATH   += include src ui

INCLUDEPATH  += . include

CONFIG       += release thread

HEADERS       = mainwindow.h \
                utils.h \
                lexer_utils.h \
                prefs.h \
                qterminal_pty.h 

SOURCES       = main.cpp \
                mainwindow.cpp \
                mainwindow_startup.cpp \
                utils.cpp \
                lexer_utils.cpp \
                prefs.cpp \
                qterminal_pty.cpp

FORMS         = dlgPrefsUI.ui

RESOURCES     = qscite.qrc

unix {
	!macx {
		LIBS         += -lqscintilla2
		DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
		SOURCES += fd_copy.c
		HEADERS += fd.h
	}
}

macx {
	TARGET = QSciTE
	LIBS += -framework qscintilla2
	DEFINES += QSCITE_MONO_FAMILY='\\"Monaco\\"'
}

win32 {
	LIBS         += -lqscintilla2
	DEFINES += QSCITE_MONO_FAMILY='"\\"Courier New\\""'
}

debug {
	CONFIG += warn_on
	DEFINES += QSCITE_DEBUG
}
