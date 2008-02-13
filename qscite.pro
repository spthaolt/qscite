CONFIG       += release

HEADERS       = mainwindow.h \
                utils.h

SOURCES       = main.cpp \
                mainwindow.cpp \
                mainwindow_startup.cpp \
                utils.cpp

RESOURCES     = qscite.qrc

!macx {
	LIBS         += -lqscintilla2
}
macx {
	LIBS += -framework qscintilla2
}

debug {
	CONFIG += warn_on
	DEFINES += QSCITE_DEBUG
}
