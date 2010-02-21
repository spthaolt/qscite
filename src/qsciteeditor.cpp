#include <QtGui>
#include <QtDebug>
#include "qsciteeditor.h"
#include "mainwindow.h"
#include <Qsci/qsciscintilla.h>

QsciteEditor::QsciteEditor(MainWindow * _parent) : QsciScintilla() {
  parent = _parent;
}

void QsciteEditor::unindentHandler() {
  beginUndoAction();

  if (hasSelectedText()) {
    int lineFrom(0), lineTo(0), indexFrom(0), indexTo(0);
    getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);

    for (int i = lineFrom; i <= lineTo; ++i) {
      unindent(i);
    }
  } else {
    int line(0), index(0);
    getCursorPosition(&line, &index);
    unindent(line);
    // attempt to return the cursor to it's original location, correcting for new indentation.
    setCursorPosition(line, index - indentationWidth());
  }

  endUndoAction();
}

void QsciteEditor::keyPressEvent(QKeyEvent * event) {
  // Shift+Tab is not supported by QScintilla, so we have to do that ourselves...
  if (event->key() == Qt::Key_Backtab) {
    unindentHandler();
  }

  QsciScintilla::keyPressEvent(event);
}

void QsciteEditor::dropEvent(QDropEvent * event) {
  parent->dropEvent(event);
}

Q_INVOKABLE void QsciteEditor::append (const QString & text ) {
  QsciScintilla::append(text);
}

Q_INVOKABLE void QsciteEditor::autoCompleteFromAll () {
  QsciScintilla::autoCompleteFromAll();
}

Q_INVOKABLE void QsciteEditor::autoCompleteFromAPIs () {
  QsciScintilla::autoCompleteFromAPIs();
}

Q_INVOKABLE void QsciteEditor::autoCompleteFromDocument () {
  QsciScintilla::autoCompleteFromDocument();
}

Q_INVOKABLE void QsciteEditor::callTip () {
  QsciScintilla::callTip();
}

Q_INVOKABLE void QsciteEditor::clear () {
  QsciScintilla::clear();
}

Q_INVOKABLE void QsciteEditor::copy () {
  QsciScintilla::copy();
}

Q_INVOKABLE void QsciteEditor::cut () {
  QsciScintilla::cut();
}

Q_INVOKABLE void QsciteEditor::ensureCursorVisible () {
  QsciScintilla::ensureCursorVisible();
}

Q_INVOKABLE void QsciteEditor::ensureLineVisible (int line ) {
  QsciScintilla::ensureLineVisible(line);
}

Q_INVOKABLE void QsciteEditor::foldLine (int line ) {
  QsciScintilla::foldLine(line);
}

Q_INVOKABLE void QsciteEditor::indent (int line ) {
  QsciScintilla::indent(line);
}

Q_INVOKABLE void QsciteEditor::insert (const QString & text ) {
  QsciScintilla::insert(text);
}

Q_INVOKABLE void QsciteEditor::insertAt (const QString & text , int line , int index ) {
  QsciScintilla::insertAt(text, line, index);
}

Q_INVOKABLE void QsciteEditor::moveToMatchingBrace () {
  QsciScintilla::moveToMatchingBrace();
}

Q_INVOKABLE void QsciteEditor::paste () {
  QsciScintilla::paste();
}

Q_INVOKABLE void QsciteEditor::redo () {
  QsciScintilla::redo();
}

Q_INVOKABLE void QsciteEditor::removeSelectedText () {
  QsciScintilla::removeSelectedText();
}

Q_INVOKABLE void QsciteEditor::resetSelectionBackgroundColor () {
  QsciScintilla::resetSelectionBackgroundColor();
}

Q_INVOKABLE void QsciteEditor::resetSelectionForegroundColor () {
  QsciScintilla::resetSelectionForegroundColor();
}

Q_INVOKABLE void QsciteEditor::selectAll (bool select) {
  QsciScintilla::selectAll(select);
}

Q_INVOKABLE void QsciteEditor::selectToMatchingBrace () {
  QsciScintilla::selectToMatchingBrace();
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionCaseSensitivity (bool cs ) {
  QsciScintilla::setAutoCompletionCaseSensitivity(cs);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionReplaceWord (bool replace ) {
  QsciScintilla::setAutoCompletionReplaceWord(replace);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionShowSingle (bool single ) {
  QsciScintilla::setAutoCompletionShowSingle(single);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionSource (QsciScintilla::AutoCompletionSource source ) {
  QsciScintilla::setAutoCompletionSource(source);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionThreshold (int thresh ) {
  QsciScintilla::setAutoCompletionThreshold(thresh);
}

Q_INVOKABLE void QsciteEditor::setAutoIndent (bool autoindent ) {
  QsciScintilla::setAutoIndent(autoindent);
}

Q_INVOKABLE void QsciteEditor::setBraceMatching (QsciScintilla::BraceMatch bm ) {
  QsciScintilla::setBraceMatching(bm);
}

Q_INVOKABLE void QsciteEditor::setBackspaceUnindents (bool unindent ) {
  QsciScintilla::setBackspaceUnindents(unindent);
}

Q_INVOKABLE void QsciteEditor::setCaretForegroundColor (const QColor & col ) {
  QsciScintilla::setCaretForegroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setCaretLineBackgroundColor (const QColor & col ) {
  QsciScintilla::setCaretLineBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setCaretLineVisible (bool enable ) {
  QsciScintilla::setCaretLineVisible(enable);
}

Q_INVOKABLE void QsciteEditor::setCaretWidth (int width ) {
  QsciScintilla::setCaretWidth(width);
}

Q_INVOKABLE void QsciteEditor::setColor (const QColor & c ) {
  QsciScintilla::setColor(c);
}

Q_INVOKABLE void QsciteEditor::setCursorPosition (int line , int index ) {
  QsciScintilla::setCursorPosition(line, index);
}

Q_INVOKABLE void QsciteEditor::setEolMode (QsciScintilla::EolMode mode ) {
  QsciScintilla::setEolMode(mode);
}

Q_INVOKABLE void QsciteEditor::setEolVisibility (bool visible ) {
  QsciScintilla::setEolVisibility(visible);
}

Q_INVOKABLE void QsciteEditor::setFolding (QsciScintilla::FoldStyle fold , int margin) {
  QsciScintilla::setFolding(fold, margin);
}

Q_INVOKABLE void QsciteEditor::setIndentation (int line , int indentation ) {
  QsciScintilla::setIndentation(line, indentation);
}

Q_INVOKABLE void QsciteEditor::setIndentationGuides (bool enable ) {
  QsciScintilla::setIndentationGuides(enable);
}

Q_INVOKABLE void QsciteEditor::setIndentationGuidesBackgroundColor (const QColor & col ) {
  QsciScintilla::setIndentationGuidesBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setIndentationGuidesForegroundColor (const QColor & col ) {
  QsciScintilla::setIndentationGuidesForegroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setIndentationsUseTabs (bool tabs ) {
  QsciScintilla::setIndentationsUseTabs(tabs);
}

Q_INVOKABLE void QsciteEditor::setIndentationWidth (int width ) {
  QsciScintilla::setIndentationWidth(width);
}

Q_INVOKABLE void QsciteEditor::setLexer (QsciLexer * lexer) {
  QsciScintilla::setLexer(lexer);
}

Q_INVOKABLE void QsciteEditor::setMarginsBackgroundColor (const QColor & col ) {
  QsciScintilla::setMarginsBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setMarginsFont (const QFont & f ) {
  QsciScintilla::setMarginsFont(f);
}

Q_INVOKABLE void QsciteEditor::setMarginsForegroundColor (const QColor & col ) {
  QsciScintilla::setMarginsForegroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setMarginLineNumbers (int margin , bool lnrs ) {
  QsciScintilla::setMarginLineNumbers(margin, lnrs);
}

Q_INVOKABLE void QsciteEditor::setMarginMarkerMask (int margin , int mask ) {
  QsciScintilla::setMarginMarkerMask(margin, mask);
}

Q_INVOKABLE void QsciteEditor::setMarginSensitivity (int margin , bool sens ) {
  QsciScintilla::setMarginSensitivity(margin, sens);
}

Q_INVOKABLE void QsciteEditor::setMarginWidth (int margin , int width ) {
  QsciScintilla::setMarginWidth(margin, width);
}

Q_INVOKABLE void QsciteEditor::setMarginWidth (int margin , const QString & s ) {
  QsciScintilla::setMarginWidth(margin, s);
}

Q_INVOKABLE void QsciteEditor::setModified (bool m ) {
  QsciScintilla::setModified(m);
}

Q_INVOKABLE void QsciteEditor::setPaper (const QColor & c ) {
  QsciScintilla::setPaper(c);
}

Q_INVOKABLE void QsciteEditor::setReadOnly (bool ro ) {
  QsciScintilla::setReadOnly(ro);
}

Q_INVOKABLE void QsciteEditor::setSelection (int lineFrom , int indexFrom , int lineTo , int indexTo ) {
  QsciScintilla::setSelection(lineFrom, indexFrom, lineTo, indexTo);
}

Q_INVOKABLE void QsciteEditor::setSelectionBackgroundColor (const QColor & col ) {
  QsciScintilla::setSelectionBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setSelectionForegroundColor (const QColor & col ) {
  QsciScintilla::setSelectionForegroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setTabIndents (bool indent ) {
  QsciScintilla::setTabIndents(indent);
}

Q_INVOKABLE void QsciteEditor::setTabWidth (int width ) {
  QsciScintilla::setTabWidth(width);
}

Q_INVOKABLE void QsciteEditor::setText (const QString & text ) {
  QsciScintilla::setText(text);
}

Q_INVOKABLE void QsciteEditor::setUtf8 (bool cp ) {
  QsciScintilla::setUtf8(cp);
}

Q_INVOKABLE void QsciteEditor::setWhitespaceVisibility (QsciScintilla::WhitespaceVisibility mode ) {
  QsciScintilla::setWhitespaceVisibility(mode);
}

Q_INVOKABLE void QsciteEditor::setWrapMode (QsciScintilla::WrapMode mode ) {
  QsciScintilla::setWrapMode(mode);
}

Q_INVOKABLE void QsciteEditor::undo () {
  QsciScintilla::undo();
}

Q_INVOKABLE void QsciteEditor::unindent (int line ) {
  QsciScintilla::unindent(line);
}

Q_INVOKABLE void QsciteEditor::zoomIn (int range ) {
  QsciScintilla::zoomIn(range);
}

Q_INVOKABLE void QsciteEditor::zoomIn () {
  QsciScintilla::zoomIn();
}

Q_INVOKABLE void QsciteEditor::zoomOut (int range ) {
  QsciScintilla::zoomOut(range);
}

Q_INVOKABLE void QsciteEditor::zoomOut () {
  QsciScintilla::zoomOut();
}

Q_INVOKABLE void QsciteEditor::zoomTo (int size ) {
  QsciScintilla::zoomTo(size);
}

Q_INVOKABLE QStringList QsciteEditor::apiContext (int pos , int & context_start , int & last_word_start ) {
  return QsciScintilla::apiContext(pos, context_start, last_word_start);
}

Q_INVOKABLE void QsciteEditor::annotate (int line , const QString & text , int style ) {
  QsciScintilla::annotate(line, text, style);
}

Q_INVOKABLE void QsciteEditor::annotate (int line , const QString & text , const QsciStyle & style ) {
  QsciScintilla::annotate(line, text, style);
}

Q_INVOKABLE void QsciteEditor::annotate (int line , const QsciStyledText & text ) {
  QsciScintilla::annotate(line, text);
}

Q_INVOKABLE void QsciteEditor::annotate (int line , const QList<QsciStyledText> & text ) {
  QsciScintilla::annotate(line, text);
}

Q_INVOKABLE QString QsciteEditor::annotation (int line ) const {
  return QsciScintilla::annotation(line);
}

Q_INVOKABLE void QsciteEditor::clearAnnotations (int line) {
  QsciScintilla::clearAnnotations(line);
}

Q_INVOKABLE bool QsciteEditor::autoCompletionCaseSensitivity () const {
  return QsciScintilla::autoCompletionCaseSensitivity();
}

Q_INVOKABLE bool QsciteEditor::autoCompletionFillupsEnabled () const {
  return QsciScintilla::autoCompletionFillupsEnabled();
}

Q_INVOKABLE bool QsciteEditor::autoCompletionReplaceWord () const {
  return QsciScintilla::autoCompletionReplaceWord();
}

Q_INVOKABLE bool QsciteEditor::autoCompletionShowSingle () const {
  return QsciScintilla::autoCompletionShowSingle();
}

Q_INVOKABLE QsciScintilla::AutoCompletionSource QsciteEditor::autoCompletionSource () const {
  return QsciScintilla::autoCompletionSource();
}

Q_INVOKABLE int QsciteEditor::autoCompletionThreshold () const {
  return QsciScintilla::autoCompletionThreshold();
}

Q_INVOKABLE bool QsciteEditor::autoIndent () const {
  return QsciScintilla::autoIndent();
}

Q_INVOKABLE bool QsciteEditor::backspaceUnindents () const {
  return QsciScintilla::backspaceUnindents();
}

Q_INVOKABLE void QsciteEditor::beginUndoAction () {
  QsciScintilla::beginUndoAction();
}

Q_INVOKABLE QsciScintilla::BraceMatch QsciteEditor::braceMatching () const {
  return QsciScintilla::braceMatching();
}

Q_INVOKABLE QsciScintilla::CallTipsStyle QsciteEditor::callTipsStyle () const {
  return QsciScintilla::callTipsStyle();
}

Q_INVOKABLE int QsciteEditor::callTipsVisible () const {
  return QsciScintilla::callTipsVisible();
}

Q_INVOKABLE void QsciteEditor::cancelList () {
  QsciScintilla::cancelList();
}

Q_INVOKABLE bool QsciteEditor::caseSensitive () const {
  return QsciScintilla::caseSensitive();
}

Q_INVOKABLE void QsciteEditor::clearRegisteredImages () {
  QsciScintilla::clearRegisteredImages();
}

Q_INVOKABLE QColor QsciteEditor::color () const {
  return QsciScintilla::color();
}

Q_INVOKABLE void QsciteEditor::convertEols (QsciScintilla::EolMode mode ) {
  QsciScintilla::convertEols(mode);
}

Q_INVOKABLE QsciDocument QsciteEditor::document () const {
  return QsciScintilla::document();
}

Q_INVOKABLE void QsciteEditor::endUndoAction () {
  QsciScintilla::endUndoAction();
}

Q_INVOKABLE QColor QsciteEditor::edgeColor () const {
  return QsciScintilla::edgeColor();
}

Q_INVOKABLE int QsciteEditor::edgeColumn () const {
  return QsciScintilla::edgeColumn();
}

Q_INVOKABLE QsciScintilla::EdgeMode QsciteEditor::edgeMode () const {
  return QsciScintilla::edgeMode();
}

Q_INVOKABLE void QsciteEditor::setFont (const QFont & f ) {
  QsciScintilla::setFont(f);
}

Q_INVOKABLE QsciScintilla::EolMode QsciteEditor::eolMode () const {
  return QsciScintilla::eolMode();
}

Q_INVOKABLE bool QsciteEditor::eolVisibility () const {
  return QsciScintilla::eolVisibility();
}

Q_INVOKABLE bool QsciteEditor::findFirst (const QString & expr , bool re , bool cs , bool wo , bool wrap , bool forward, int line, int index, bool show) {
  return QsciScintilla::findFirst(expr, re, cs, wo, wrap, forward, line, index, show);
}

Q_INVOKABLE bool QsciteEditor::findNext () {
  return QsciScintilla::findNext();
}

Q_INVOKABLE int QsciteEditor::firstVisibleLine () const {
  return QsciScintilla::firstVisibleLine();
}

Q_INVOKABLE void QsciteEditor::foldAll (bool children) {
  QsciScintilla::foldAll(children);
}

Q_INVOKABLE QsciScintilla::FoldStyle QsciteEditor::folding () const {
  return QsciScintilla::folding();
}

Q_INVOKABLE void QsciteEditor::getCursorPosition (int * line , int * index ) const {
  QsciScintilla::getCursorPosition(line, index);
}

Q_INVOKABLE void QsciteEditor::getSelection (int * lineFrom , int * indexFrom , int * lineTo , int * indexTo ) const {
  QsciScintilla::getSelection(lineFrom, indexFrom, lineTo, indexTo);
}

Q_INVOKABLE bool QsciteEditor::hasSelectedText () const {
  return QsciScintilla::hasSelectedText();
}

Q_INVOKABLE int QsciteEditor::indentation (int line ) const {
  return QsciScintilla::indentation(line);
}

Q_INVOKABLE bool QsciteEditor::indentationGuides () const {
  return QsciScintilla::indentationGuides();
}

Q_INVOKABLE bool QsciteEditor::indentationsUseTabs () const {
  return QsciScintilla::indentationsUseTabs();
}

Q_INVOKABLE int QsciteEditor::indentationWidth () const {
  return QsciScintilla::indentationWidth();
}

Q_INVOKABLE bool QsciteEditor::isCallTipActive () const {
  return QsciScintilla::isCallTipActive();
}

Q_INVOKABLE bool QsciteEditor::isListActive () const {
  return QsciScintilla::isListActive();
}

Q_INVOKABLE bool QsciteEditor::isModified () const {
  return QsciScintilla::isModified();
}

Q_INVOKABLE bool QsciteEditor::isReadOnly () const {
  return QsciScintilla::isReadOnly();
}

Q_INVOKABLE bool QsciteEditor::isRedoAvailable () const {
  return QsciScintilla::isRedoAvailable();
}

Q_INVOKABLE bool QsciteEditor::isUndoAvailable () const {
  return QsciScintilla::isUndoAvailable();
}

Q_INVOKABLE bool QsciteEditor::isUtf8 () const {
  return QsciScintilla::isUtf8();
}

Q_INVOKABLE bool QsciteEditor::isWordCharacter (char ch ) const {
  return QsciScintilla::isWordCharacter(ch);
}

Q_INVOKABLE int QsciteEditor::lineAt (const QPoint & pos ) const {
  return QsciScintilla::lineAt(pos);
}

Q_INVOKABLE void QsciteEditor::lineIndexFromPosition (int position , int * line , int * index ) const {
  QsciScintilla::lineIndexFromPosition(position, line, index);
}

Q_INVOKABLE int QsciteEditor::lineLength (int line ) const {
  return QsciScintilla::lineLength(line);
}

Q_INVOKABLE int QsciteEditor::lines () const {
  return QsciScintilla::lines();
}

Q_INVOKABLE int QsciteEditor::length () const {
  return QsciScintilla::length();
}

Q_INVOKABLE QsciLexer * QsciteEditor::lexer () const {
  return QsciScintilla::lexer();
}

Q_INVOKABLE bool QsciteEditor::marginLineNumbers (int margin ) const {
  return QsciScintilla::marginLineNumbers(margin);
}

Q_INVOKABLE int QsciteEditor::marginMarkerMask (int margin ) const {
  return QsciScintilla::marginMarkerMask(margin);
}

Q_INVOKABLE bool QsciteEditor::marginSensitivity (int margin ) const {
  return QsciScintilla::marginSensitivity(margin);
}

Q_INVOKABLE QsciScintilla::MarginType QsciteEditor::marginType (int margin ) const {
  return QsciScintilla::marginType(margin);
}

Q_INVOKABLE int QsciteEditor::marginWidth (int margin ) const {
  return QsciScintilla::marginWidth(margin);
}

Q_INVOKABLE int QsciteEditor::markerDefine (char ch , int mnr) {
  return QsciScintilla::markerDefine(ch, mnr);
}

Q_INVOKABLE int QsciteEditor::markerDefine (const QPixmap & pm , int mnr) {
  return QsciScintilla::markerDefine(pm, mnr);
}

Q_INVOKABLE int QsciteEditor::markerAdd (int linenr , int mnr ) {
  return QsciScintilla::markerAdd(linenr, mnr);
}

Q_INVOKABLE unsigned QsciteEditor::markersAtLine (int linenr ) const {
  return QsciScintilla::markersAtLine(linenr);
}

Q_INVOKABLE void QsciteEditor::markerDelete (int linenr , int mnr) {
  QsciScintilla::markerDelete(linenr, mnr);
}

Q_INVOKABLE void QsciteEditor::markerDeleteAll (int mnr) {
  QsciScintilla::markerDeleteAll(mnr);
}

Q_INVOKABLE void QsciteEditor::markerDeleteHandle (int mhandle ) {
  QsciScintilla::markerDeleteHandle(mhandle);
}

Q_INVOKABLE int QsciteEditor::markerLine (int mhandle ) const {
  return QsciScintilla::markerLine(mhandle);
}

Q_INVOKABLE int QsciteEditor::markerFindNext (int linenr , unsigned mask ) const {
  return QsciScintilla::markerFindNext(linenr, mask);
}

Q_INVOKABLE int QsciteEditor::markerFindPrevious (int linenr , unsigned mask ) const {
  return QsciScintilla::markerFindPrevious(linenr, mask);
}

Q_INVOKABLE QColor QsciteEditor::paper () const {
  return QsciScintilla::paper();
}

Q_INVOKABLE int QsciteEditor::positionFromLineIndex (int line , int index ) const {
  return QsciScintilla::positionFromLineIndex(line, index);
}

Q_INVOKABLE bool QsciteEditor::read (QIODevice * io ) {
  return QsciScintilla::read(io);
}

Q_INVOKABLE void QsciteEditor::recolor (int start, int end ) {
  QsciScintilla::recolor(start, end);
}

Q_INVOKABLE void QsciteEditor::registerImage (int id , const QPixmap & pm ) {
  QsciScintilla::registerImage(id, pm);
}

Q_INVOKABLE void QsciteEditor::replace (const QString & replaceStr ) {
  QsciScintilla::replace(replaceStr);
}

Q_INVOKABLE void QsciteEditor::resetFoldMarginColors () {
  QsciScintilla::resetFoldMarginColors();
}

Q_INVOKABLE void QsciteEditor::setFoldMarginColors (const QColor & fore , const QColor & back ) {
  QsciScintilla::setFoldMarginColors(fore, back);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionFillupsEnabled (bool enabled ) {
  QsciScintilla::setAutoCompletionFillupsEnabled(enabled);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionFillups (const char * fillups ) {
  QsciScintilla::setAutoCompletionFillups(fillups);
}

Q_INVOKABLE void QsciteEditor::setAutoCompletionWordSeparators (const QStringList & separators ) {
  QsciScintilla::setAutoCompletionWordSeparators(separators);
}

Q_INVOKABLE void QsciteEditor::setCallTipsBackgroundColor (const QColor & col ) {
  QsciScintilla::setCallTipsBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setCallTipsForegroundColor (const QColor & col ) {
  QsciScintilla::setCallTipsForegroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setCallTipsHighlightColor (const QColor & col ) {
  QsciScintilla::setCallTipsHighlightColor(col);
}

Q_INVOKABLE void QsciteEditor::setCallTipsStyle (QsciScintilla::CallTipsStyle style ) {
  QsciScintilla::setCallTipsStyle(style);
}

Q_INVOKABLE void QsciteEditor::setCallTipsVisible (int nr ) {
  QsciScintilla::setCallTipsVisible(nr);
}

Q_INVOKABLE void QsciteEditor::setDocument (const QsciDocument & document ) {
  QsciScintilla::setDocument(document);
}

Q_INVOKABLE void QsciteEditor::setEdgeColor (const QColor & col ) {
  QsciScintilla::setEdgeColor(col);
}

Q_INVOKABLE void QsciteEditor::setEdgeColumn (int colnr ) {
  QsciScintilla::setEdgeColumn(colnr);
}

Q_INVOKABLE void QsciteEditor::setEdgeMode (QsciScintilla::EdgeMode mode ) {
  QsciScintilla::setEdgeMode(mode);
}

Q_INVOKABLE void QsciteEditor::setMarginText (int line , const QString & text , int style ) {
  QsciScintilla::setMarginText(line, text, style);
}

Q_INVOKABLE void QsciteEditor::setMarginText (int line , const QString & text , const QsciStyle & style ) {
  QsciScintilla::setMarginText(line, text, style);
}

Q_INVOKABLE void QsciteEditor::setMarginText (int line , const QsciStyledText & text ) {
  QsciScintilla::setMarginText(line, text);
}

Q_INVOKABLE void QsciteEditor::setMarginType (int margin , QsciScintilla::MarginType type ) {
  QsciScintilla::setMarginType(margin, type);
}

Q_INVOKABLE void QsciteEditor::clearMarginText (int line) {
  QsciScintilla::clearMarginText(line);
}

Q_INVOKABLE void QsciteEditor::setMarkerBackgroundColor (const QColor & col , int mnr) {
  QsciScintilla::setMarkerBackgroundColor(col, mnr);
}

Q_INVOKABLE void QsciteEditor::setMarkerForegroundColor (const QColor & col , int mnr) {
  QsciScintilla::setMarkerForegroundColor(col, mnr);
}

Q_INVOKABLE void QsciteEditor::setMatchedBraceBackgroundColor (const QColor & col ) {
  QsciScintilla::setMatchedBraceBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setMatchedBraceForegroundColor (const QColor & col ) {
  QsciScintilla::setMatchedBraceForegroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setUnmatchedBraceBackgroundColor (const QColor & col ) {
  QsciScintilla::setUnmatchedBraceBackgroundColor(col);
}

Q_INVOKABLE void QsciteEditor::setUnmatchedBraceForegroundColor (const QColor & col ) {
  QsciScintilla::setUnmatchedBraceForegroundColor(col);
}

Q_INVOKABLE QString QsciteEditor::selectedText () const {
  return QsciScintilla::selectedText();
}

Q_INVOKABLE bool QsciteEditor::selectionToEol () const {
  return QsciScintilla::selectionToEol();
}

Q_INVOKABLE void QsciteEditor::setSelectionToEol (bool filled ) {
  QsciScintilla::setSelectionToEol(filled);
}

Q_INVOKABLE void QsciteEditor::showUserList (int id , const QStringList & list ) {
  QsciScintilla::showUserList(id, list);
}

Q_INVOKABLE QsciCommandSet * QsciteEditor::standardCommands () const {
  return QsciScintilla::standardCommands();
}

Q_INVOKABLE bool QsciteEditor::tabIndents () const {
  return QsciScintilla::tabIndents();
}

Q_INVOKABLE int QsciteEditor::tabWidth () const {
  return QsciScintilla::tabWidth();
}

Q_INVOKABLE QString QsciteEditor::text () const {
  return QsciScintilla::text();
}

Q_INVOKABLE QString QsciteEditor::text (int line ) const {
  return QsciScintilla::text(line);
}

Q_INVOKABLE int QsciteEditor::textHeight (int linenr ) const {
  return QsciScintilla::textHeight(linenr);
}

Q_INVOKABLE QsciScintilla::WhitespaceVisibility QsciteEditor::whitespaceVisibility () const {
  return QsciScintilla::whitespaceVisibility();
}

Q_INVOKABLE QString QsciteEditor::wordAtPoint (const QPoint & point ) const {
  return QsciScintilla::wordAtPoint(point);
}

Q_INVOKABLE const char * QsciteEditor::wordCharacters () const {
  return QsciScintilla::wordCharacters();
}

Q_INVOKABLE QsciScintilla::WrapMode QsciteEditor::wrapMode () const {
  return QsciScintilla::wrapMode();
}

Q_INVOKABLE bool QsciteEditor::write (QIODevice * io ) const {
  return QsciScintilla::write(io);
}
