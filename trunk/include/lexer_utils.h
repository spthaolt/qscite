#ifndef _QSCITE_LEXER_UTILS_H
#define _QSCITE_LEXER_UTILS_H

#include <QString>
class QsciLexer;
class QSettings;

QsciLexer* getLexerForDocument(const QString & fileName, const QString & text);

QsciLexer * getLexerByName(const QString & lexerName);

void setLexerFont(QsciLexer * lexer, const QString & family, int size);

void writeDefaultExtensions(QSettings & settings);
void writeDefaultMagic(QSettings & settings);

extern const QString supportedLexers[];

#endif
