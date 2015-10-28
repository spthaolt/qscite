# Dependencies #

## Qt 4 ##

My installed version of Qt is 4.3.2. Qt 4.2 may be usable, but it has not been tested. Any Qt older than 4.2 is almost guaranteed not to work.

## QScintilla ##

We use the 20080209 snapshot of QScintilla 2, currently available from http://www.riverbankcomputing.com/Downloads/Snapshots/QScintilla2/.

The QSciTE project file assumes that QScintilla has been built as a framework on OS X. QScintilla does not build a framework by default; add 'CONFIG+=framework' to the appropriate project file or to the qmake command line.

# Idiosyncrasies #

If QSciTE fails to open dropped files after updating the app bundle (`cp -R macx-bundle-contents/* QSciTE.app/Contents/`), drag QSciTE.app to another folder or the desktop to cause the system to re-read the bundle properties. It can safely be moved back to the original location once the change has taken effect.