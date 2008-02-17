CONFIG       += release

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
		LIBS         += -lqscintilla2
		DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
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
