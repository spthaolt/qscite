#include "lexer_utils.h"

#include <QString>
#include <QSettings>
#include <QFont>
#include <QtDebug>

#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>

#include <cassert>

/* String keys for each lexer we support. Note empty string marking end of 
 * array. Please keep these in alphabetical order.
 */
const QString supportedLexers[] = {
	"bash", "cpp", "csharp", "css", "html", "java", "perl", "pov", "python",
	"ruby", "" 
};

QsciLexer * getLexerByName(const QString & lexerName) {
	if (lexerName == "bash") {
		return new QsciLexerBash();
	} else if (lexerName == "cpp") {
		return new QsciLexerCPP();
	} else if (lexerName == "csharp") {
		return new QsciLexerCSharp();
	} else if (lexerName == "css") {
		return new QsciLexerCSS();
	} else if (lexerName == "html") {
		return new QsciLexerHTML();
	} else if (lexerName == "java") {
		return new QsciLexerJava();
	} else if (lexerName == "perl") {
		return new QsciLexerPerl();
	} else if (lexerName == "pov") {
		return new QsciLexerPOV();
	} else if (lexerName == "python") {
		return new QsciLexerPython();
	} else if (lexerName == "ruby") {
		return new QsciLexerRuby();
	} else {
		return NULL;
	}
}

int indexOfLexer(const QString & lexerName) {
	for (int i = 0; !supportedLexers[i].isEmpty(); ++i) {
		if (lexerName == supportedLexers[i]) {
			return i;
		}
	}
	return -1;
}

namespace {	
	/* Default extensions for each lexer (in order listed in supportedLexers[]).
	 * End of extensions for each lexer marked with "".
	 */
	const QString defaultExtensions[] = {
		/* bash   */ "sh", "",
		/* cpp    */ "c", "h", "cc", "cpp", "cxx", "",
		/* csharp */ "cs", "",
		/* css    */ "css","",
		/* html   */ "html", "xhtml", "xml", "plist", "",
		/* java   */ "java", "",
		/* perl   */ "pl", "pm", "",
		/* pov    */ "pov", "inc", "",
		/* python */ "py", "pyw", "",
		/* ruby   */ "rb", ""
	};
	
	/* Default magic values for identifying files we can't identify by
	 * extension. Magic values must appear somewhere in the first line of the
	 * file. Must be in order and "" terminated, as above.
	 */
	/* TODO: these will probably be regexes someday. */
	const QString defaultMagic[] = {
		/* bash   */ "/sh", "bash", "",
		/* cpp    */ "",
		/* csharp */ "",
		/* css    */ "",
		/* html   */ "<?xml", "<!DOCTYPE", "",
		/* java   */ "",
		/* perl   */ "perl", "",
		/* pov    */ "",
		/* python */ "python", "",
		/* ruby   */ "ruby", ""
	};	
} // namespace	
	
void writeDefaultExtensions(QSettings & settings) {
	assert(settings.group() == "extensions");

	for (int i = 0, j = 0; !supportedLexers[i].isEmpty(); ++i, ++j) {
		qDebug() << qPrintable(supportedLexers[i]);
		settings.beginWriteArray(supportedLexers[i]);
		
		for (int k = 0; !defaultExtensions[j].isEmpty(); ++j, ++k) {
			settings.setArrayIndex(k);
			qDebug() << k << ": " << qPrintable(defaultExtensions[j]) << ' ';
			settings.setValue("ext", defaultExtensions[j]);
		}
		
		settings.endArray();
	}
	settings.setValue("version", 1);
} // writeDefaultExtensions

void writeDefaultMagic(QSettings & settings) {
	assert(settings.group() == "magic");

	for (int i = 0, j = 0; !supportedLexers[i].isEmpty(); ++i, ++j) {
		qDebug() << qPrintable(supportedLexers[i]);
		settings.beginWriteArray(supportedLexers[i]);
		
		for (int k = 0; !defaultMagic[j].isEmpty(); ++j, ++k) {
			settings.setArrayIndex(k);
			qDebug() << k << ": " << qPrintable(defaultMagic[j]) << ' ';
			settings.setValue("str", defaultMagic[j]);
		}
		
		settings.endArray();
	}
	settings.setValue("version", 1);
} // writeDefaultMagic

QsciLexer* getLexerForDocument(const QString& fileName, const QString& text) {
	// get the file extension
    int lastDot = fileName.lastIndexOf('.');
    QString ext = (lastDot > 0) ? fileName.right(fileName.size() - lastDot - 1) : "";
    /* TODO: do we need to notice newline mode here? */
	QString firstLine = text.left(text.indexOf('\n'));
	
	QSettings settings;
	int arrSize;
	
	if (!ext.isEmpty()) { // We have an extension. See if it matches anything.
		settings.beginGroup("extensions");
		
		if (settings.value("version", 0).toInt() < 1) {
			qDebug() << "Using default extension mappings";
			writeDefaultExtensions(settings);
		}

		for (int i = 0; !supportedLexers[i].isEmpty(); ++i) {
			arrSize = settings.beginReadArray(supportedLexers[i]);
			qDebug() << "Found" << arrSize << "extensions for" << supportedLexers[i];
			for (int j = 0; j < arrSize; ++j) {
				settings.setArrayIndex(j);
				if (ext == settings.value("ext", "").toString()) {
					return getLexerByName(supportedLexers[i]);
				}
			}
			settings.endArray();
		}
		settings.endGroup();
	}
	
	/*
	 * No extension or unmatched extension. Use magic.
	 */
	qDebug() << "Using magic";
	settings.beginGroup("magic");

	if (!settings.value("version", 0).toInt()) {
		qDebug() << "Using default magic";
		writeDefaultMagic(settings);
	}

	for (int i = 0; !supportedLexers[i].isEmpty(); ++i) {
		arrSize = settings.beginReadArray(supportedLexers[i]);
		qDebug() << "Found" << arrSize << "magics for" << supportedLexers[i];
		for (int j = 0; j < arrSize; ++j) {
			settings.setArrayIndex(j);
			if (firstLine.contains(settings.value("str", "").toString())) {
				return getLexerByName(supportedLexers[i]);
			}
		}
		settings.endArray();
	}
	
	settings.endGroup();
		
	/*
	 * Couldn't identify extension or magic.
	 * Return no lexer, indicating plain text.
	 */
  qDebug() << "No appropriate lexer found; using plain text.";
	return NULL;
}

#define MAX_STYLE_IDX 127

void setLexerFont(QsciLexer * lexer, const QString & family, int size) {
	for (int i = 0; i <= MAX_STYLE_IDX; ++i) {
		if (!(lexer->description(i)).isEmpty()) { // This style is in use
			QFont curFont = lexer->font(i);
			curFont.setFamily(family);
			curFont.setPointSize(size);
			lexer->setFont(curFont, i);
		}
	}
}
