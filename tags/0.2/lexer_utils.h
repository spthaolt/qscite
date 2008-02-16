#include <QString>
class QsciLexer;

QsciLexer* getLexerForDocument(const QString & fileName, const QString & text);

void setLexerFont(QsciLexer * lexer, const QString & family, int size);
