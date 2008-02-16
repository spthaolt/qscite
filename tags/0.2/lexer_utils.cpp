#include "lexer_utils.h"

#include <QString>
#include <QSettings>
#include <QFont>

#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>

#ifdef QSCITE_DEBUG
#include <iostream>
#endif

#include <cassert>

namespace {
	/* String keys for each lexer we support. Note empty string marking end of 
	 * array. Please keep these in alphabetical order.
	 */
	const QString supportedLexers[] = {
		"bash", "cpp", "csharp", "css", "html", "java", "perl", "python",
		"ruby", "" 
	};
	
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
		/* python */ "python", "",
		/* ruby   */ "ruby", ""
	};	
	
	
	void writeDefaultExtensions(QSettings & settings) {
		assert(settings.group() == "extensions");

		for (int i = 0, j = 0; !supportedLexers[i].isEmpty(); ++i, ++j) {
#ifdef QSCITE_DEBUG
			std::cout << qPrintable(supportedLexers[i]) << std::endl;
#endif
			settings.beginWriteArray(supportedLexers[i]);
			for (int k = 0; !defaultExtensions[j].isEmpty(); ++j, ++k) {
				settings.setArrayIndex(k);
#ifdef QSCITE_DEBUG
				std::cout << k << ": " << qPrintable(defaultExtensions[j]) << ' ';
#endif
				settings.setValue("ext", defaultExtensions[j]);
			}
			settings.endArray();
#ifdef QSCITE_DEBUG
			std::cout << std::endl;
#endif
		}
		
		settings.setValue("version", 1);
	} // writeDefaultExtensions

	void writeDefaultMagic(QSettings & settings) {
		assert(settings.group() == "magic");

		for (int i = 0, j = 0; !supportedLexers[i].isEmpty(); ++i, ++j) {
#ifdef QSCITE_DEBUG
			std::cout << qPrintable(supportedLexers[i]) << std::endl;
#endif
			settings.beginWriteArray(supportedLexers[i]);
			for (int k = 0; !defaultMagic[j].isEmpty(); ++j, ++k) {
				settings.setArrayIndex(k);
#ifdef QSCITE_DEBUG
				std::cout << k << ": " << qPrintable(defaultMagic[j]) << ' ';
#endif
				settings.setValue("str", defaultMagic[j]);
			}
			settings.endArray();
#ifdef QSCITE_DEBUG
			std::cout << std::endl;
#endif
		}
		
		settings.setValue("version", 1);
	} // writeDefaultMagic
} // namespace

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
#ifdef QSCITE_DEBUG
			std::cout << "Using default extension mappings" << std::endl;
#endif
			writeDefaultExtensions(settings);
		}
		
		/* This is way too much repeated code. Is there any way to get a
		 * reference to a class in C++?
		 *
		 * Please keep the languages in alphabetical order here too.
		 */

		arrSize = settings.beginReadArray("bash");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for bash" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerBash();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("cpp");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for CPP" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerCPP();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("csharp");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for CSharp" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerCSharp();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("css");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for CSS" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerCSS();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("html");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for HTML" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerHTML();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("java");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for Java" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerJava();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("perl");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for Perl" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerPerl();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("python");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for Python" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerPython();
			}
		}
		settings.endArray();
		
		arrSize = settings.beginReadArray("ruby");
#ifdef QSCITE_DEBUG
		std::cout << "Found " << arrSize << " extensions for Ruby" << std::endl;
#endif
		for (int i = 0; i < arrSize; ++i) {
			settings.setArrayIndex(i);
			if (ext == settings.value("ext","").toString()) {
				return new QsciLexerRuby();
			}
		}
		settings.endArray();
		
		settings.endGroup();
	}
	
	/*
	 * No extension or unmatched extension. Use magic.
	 */
#ifdef QSCITE_DEBUG
	std::cout << "Using magic" << std::endl;
#endif
	settings.beginGroup("magic");
	if (!settings.value("version", 0).toInt()) {
#ifdef QSCITE_DEBUG
		std::cout << "Using default magic" << std::endl;
#endif
		writeDefaultMagic(settings);
	}

	arrSize = settings.beginReadArray("bash");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for bash" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerBash();
		}
	}
	settings.endArray();
	
	arrSize = settings.beginReadArray("css");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for CSS" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerCSS();
		}
	}
	settings.endArray();
	
	arrSize = settings.beginReadArray("html");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for HTML" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerHTML();
		}
	}
	settings.endArray();
	
	arrSize = settings.beginReadArray("java");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for Java" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerJava();
		}
	}
	settings.endArray();
	
	arrSize = settings.beginReadArray("perl");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for Perl" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerPerl();
		}
	}
	settings.endArray();
	
	arrSize = settings.beginReadArray("python");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for Python" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerPython();
		}
	}
	settings.endArray();
	
	arrSize = settings.beginReadArray("ruby");
#ifdef QSCITE_DEBUG
	std::cout << "Found " << arrSize << " magics for Ruby" << std::endl;
#endif
	for (int i = 0; i < arrSize; ++i) {
		settings.setArrayIndex(i);
		if (firstLine.contains(settings.value("str").toString())) {
			return new QsciLexerRuby();
		}
	}
	settings.endArray();
	
	settings.endGroup();
		
	/*
	 * Couldn't identify extension or magic.
	 * Return no lexer, indicating plain text.
	 */
	return 0;
}

#define MAX_STYLE_IDX 50

void setLexerFont(QsciLexer * lexer, const QString & family, int size) {
	for (int i = 0; i < MAX_STYLE_IDX; ++i) {
		if (!(lexer->description(i)).isEmpty()) { // This style is in use
			QFont curFont = lexer->font(i);
			curFont.setFamily(family);
			curFont.setPointSize(size);
			lexer->setFont(curFont, i);
		}
	}
}
