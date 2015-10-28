## Project Status (February 2010) ##
I've branched the project to work on replacing our own QTerminal classes with QTermWidget that I found else where (it's a Qt4 port of the Konsole code from KDE).  It's much more complete and usable than our QTerminal widget was going to be.  Since the QTermWidget project seems to have stagnated, I'm forking it here to fix bugs and add features as necessary.

If you've stumbled on this project and you want to either help in development or suggest a feature, don't hesitate to email me: jpsutton@gmail.com.

I'd also like to hear from any that are actually using QSciTE in the real world, as that's what we're developing it for. :)

## Project Summary ##
QSciTE is a clone of the popular SciTE text/code editor.  It's written in Qt4.

I've created this project for several reasons.  First, I was using Mac OS X, and was horrified to find that SciTE was not supported.  Second, I was annoyed that SciTE interface differed quite a bit between the Linux and Win32 implementations.

I put the project in a folder and forgot about it for a while.  After a friend asked me if I knew of a nice Linux code editor with a similar interface as SciTE (besides the GTK port of SciTE itself), I decided to revive it and add more features.

This project utilizes GNU C++ with the Qt4 interface library. In addition, we're building on the roots of the original SciTE by using the QScintilla2 library, a Qt4 port of the Scintilla library used to build SciTE.

Where ever possible, we're trying to keep the code cross-platform, however, given the lack of general compatibility between Unix-like operating systems and Windows, we will be forced to implement some features (such as the built-in shell) for Linux and Mac OS X, while maintaining a completely different implementation for Windows.

## Latest Release ##
The latest release is 0.4 (pretty old, you may want to compile from svn...we try to keep the current trunk compilable).

Pre-compiled binaries are available only for Win32 and Mac OS X (10.4 PPC) at the moment.  Linux binaries _may_ be added in the future if we can get the Qt4 and QScintilla2 libraries to compile statically.  If you're a Linux user, I suspect you probably already know how to compile applications like this; however, if you need a few pointers, we've documented a common procedure [here](http://code.google.com/p/qscite/wiki/BuildLinux). :)

This release is still considered alpha, as some features have not been fully implemented yet (such as proper find/replace support).  New features in this release include...

  * Win32 Terminal support
    * Backspace is properly supported (unlike the original SciTE)
    * Command history is supported (within the current session)
  * Files now encoded with UTF-8 when saving
  * Command-line parameter support (allows for file associations as well)
  * View/Convert line endings between CR/LF/CRLF
  * Convert indentation on a file (including conversion from tabs to spaces)
  * System tray is now enabled on supporting systems
  * Default directory when opening/saving is now a lot smarter
  * Code folding support (currently has to be enabled per-document)
  * A Find Text dialog has been added (also supports regular expressions!)
  * A LOT of general bug fixes to improve your code editing experience!
  * There are probably more features; if you want to learn more, look through the SVN commit logs :)

Have fun!