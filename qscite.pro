CONFIG       += release

HEADERS       = mainwindow.h \
                utils.h \
                lexer_utils.h

SOURCES       = main.cpp \
                mainwindow.cpp \
                mainwindow_startup.cpp \
                utils.cpp \
                lexer_utils.cpp

RESOURCES     = qscite.qrc

unix {
	!macx {
		LIBS         += -lqscintilla2
		DEFINES += QSCITE_MONO_FAMILY='\\"Monospace\\"'
	}
}

macx {
	LIBS += -framework qscintilla2
	DEFINES += QSCITE_MONO_FAMILY='\\"Monaco\\"'
}

win32 {
	LIBS         += -lqscintilla2
	#FIXME: Does the UNIX way of getting quotes into a define work on Win32?
	DEFINES += QSCITE_MONO_FAMILY='\\"Courier New\\"'
}

debug {
	CONFIG += warn_on
	DEFINES += QSCITE_DEBUG
}
