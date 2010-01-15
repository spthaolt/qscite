/**
 * Wrapper to pass to script to allow for access to more methods.
 */
#ifndef QSCITE_EDITOR_SCRIPT_WRAPPER_H_
#define QSCITE_EDITOR_SQsciScintillaCRIPT_WRAPPER_H_

#include <QtGui>
#include <QtDebug>
#include <QString>

#include <Qsci/qsciscintilla.h>
#include "qsciteeditor.h"

class QSciteEditorScriptWrapper : public QObject {
  
  Q_OBJECT
  
  protected:
    QsciteEditor* editor;
  
  public:
    QSciteEditorScriptWrapper(QsciteEditor* ed) {
      editor = ed;
    }
    
    Q_INVOKABLE void append (const QString & text ) {
      return editor->append(text);
    }

    Q_INVOKABLE void autoCompleteFromAll () {
      return editor->autoCompleteFromAll();
    }

    Q_INVOKABLE void autoCompleteFromAPIs () {
      return editor->autoCompleteFromAPIs();
    }

    Q_INVOKABLE void autoCompleteFromDocument () {
      return editor->autoCompleteFromDocument();
    }

    Q_INVOKABLE void callTip () {
      return editor->callTip();
    }

    Q_INVOKABLE void clear () {
      return editor->clear();
    }

    Q_INVOKABLE void copy () {
      return editor->copy();
    }

    Q_INVOKABLE void cut () {
      return editor->cut();
    }

    Q_INVOKABLE void ensureCursorVisible () {
      return editor->ensureCursorVisible();
    }

    Q_INVOKABLE void ensureLineVisible (int line ) {
      return editor->ensureLineVisible(line);
    }

    Q_INVOKABLE void foldAll (bool children = false ) {
      return editor->foldAll(children);
    }

    Q_INVOKABLE void foldLine (int line ) {
      return editor->foldLine(line);
    }

    Q_INVOKABLE void indent (int line ) {
      return editor->indent(line);
    }

    Q_INVOKABLE void insert (const QString & text ) {
      return editor->insert(text);
    }

    Q_INVOKABLE void insertAt (const QString & text , int line , int index ) {
      return editor->insertAt(text, line, index);
    }

    Q_INVOKABLE void moveToMatchingBrace () {
      return editor->moveToMatchingBrace();
    }

    Q_INVOKABLE void paste () {
      return editor->paste();
    }

    Q_INVOKABLE void redo () {
      return editor->redo();
    }

    Q_INVOKABLE void removeSelectedText () {
      return editor->removeSelectedText();
    }

    Q_INVOKABLE void resetSelectionBackgroundColor () {
      return editor->resetSelectionBackgroundColor();
    }

    Q_INVOKABLE void resetSelectionForegroundColor () {
      return editor->resetSelectionForegroundColor();
    }

    Q_INVOKABLE void selectAll (bool select = true ) {
      return editor->selectAll(select);
    }

    Q_INVOKABLE void selectToMatchingBrace () {
      return editor->selectToMatchingBrace();
    }

    Q_INVOKABLE void setAutoCompletionCaseSensitivity (bool cs ) {
      return editor->setAutoCompletionCaseSensitivity(cs);
    }

    Q_INVOKABLE void setAutoCompletionReplaceWord (bool replace ) {
      return editor->setAutoCompletionReplaceWord(replace);
    }

    Q_INVOKABLE void setAutoCompletionShowSingle (bool single ) {
      return editor->setAutoCompletionShowSingle(single);
    }

    Q_INVOKABLE void setAutoCompletionSource (QsciScintilla::AutoCompletionSource source ) {
      return editor->setAutoCompletionSource(source);
    }

    Q_INVOKABLE void setAutoCompletionThreshold (int thresh ) {
      return editor->setAutoCompletionThreshold(thresh);
    }

    Q_INVOKABLE void setAutoIndent (bool autoindent ) {
      return editor->setAutoIndent(autoindent);
    }

    Q_INVOKABLE void setBraceMatching (QsciScintilla::BraceMatch bm ) {
      return editor->setBraceMatching(bm);
    }

    Q_INVOKABLE void setBackspaceUnindents (bool unindent ) {
      return editor->setBackspaceUnindents(unindent);
    }

    Q_INVOKABLE void setCaretForegroundColor (const QColor & col ) {
      return editor->setCaretForegroundColor(col);
    }

    Q_INVOKABLE void setCaretLineBackgroundColor (const QColor & col ) {
      return editor->setCaretLineBackgroundColor(col);
    }

    Q_INVOKABLE void setCaretLineVisible (bool enable ) {
      return editor->setCaretLineVisible(enable);
    }

    Q_INVOKABLE void setCaretWidth (int width ) {
      return editor->setCaretWidth(width);
    }

    Q_INVOKABLE void setColor (const QColor & c ) {
      return editor->setColor(c);
    }

    Q_INVOKABLE void setCursorPosition (int line , int index ) {
      return editor->setCursorPosition(line, index);
    }

    Q_INVOKABLE void setEolMode (QsciScintilla::EolMode mode ) {
      return editor->setEolMode(mode);
    }

    Q_INVOKABLE void setEolVisibility (bool visible ) {
      return editor->setEolVisibility(visible);
    }

    Q_INVOKABLE void setFolding (QsciScintilla::FoldStyle fold , int margin = 2 ) {
      return editor->setFolding(fold, margin);
    }

    Q_INVOKABLE void setIndentation (int line , int indentation ) {
      return editor->setIndentation(line, indentation);
    }

    Q_INVOKABLE void setIndentationGuides (bool enable ) {
      return editor->setIndentationGuides(enable);
    }

    Q_INVOKABLE void setIndentationGuidesBackgroundColor (const QColor & col ) {
      return editor->setIndentationGuidesBackgroundColor(col);
    }

    Q_INVOKABLE void setIndentationGuidesForegroundColor (const QColor & col ) {
      return editor->setIndentationGuidesForegroundColor(col);
    }

    Q_INVOKABLE void setIndentationsUseTabs (bool tabs ) {
      return editor->setIndentationsUseTabs(tabs);
    }

    Q_INVOKABLE void setIndentationWidth (int width ) {
      return editor->setIndentationWidth(width);
    }

    Q_INVOKABLE void setLexer (QsciLexer * lexer = 0 ) {
      return editor->setLexer(lexer);
    }

    Q_INVOKABLE void setMarginsBackgroundColor (const QColor & col ) {
      return editor->setMarginsBackgroundColor(col);
    }

    Q_INVOKABLE void setMarginsFont (const QFont & f ) {
      return editor->setMarginsFont(f);
    }

    Q_INVOKABLE void setMarginsForegroundColor (const QColor & col ) {
      return editor->setMarginsForegroundColor(col);
    }

    Q_INVOKABLE void setMarginLineNumbers (int margin , bool lnrs ) {
      return editor->setMarginLineNumbers(margin, lnrs);
    }

    Q_INVOKABLE void setMarginMarkerMask (int margin , int mask ) {
      return editor->setMarginMarkerMask(margin, mask);
    }

    Q_INVOKABLE void setMarginSensitivity (int margin , bool sens ) {
      return editor->setMarginSensitivity(margin, sens);
    }

    Q_INVOKABLE void setMarginWidth (int margin , int width ) {
      return editor->setMarginWidth(margin, width);
    }

    Q_INVOKABLE void setMarginWidth (int margin , const QString & s ) {
      return editor->setMarginWidth(margin, s);
    }

    Q_INVOKABLE void setModified (bool m ) {
      return editor->setModified(m);
    }

    Q_INVOKABLE void setPaper (const QColor & c ) {
      return editor->setPaper(c);
    }

    Q_INVOKABLE void setReadOnly (bool ro ) {
      return editor->setReadOnly(ro);
    }

    Q_INVOKABLE void setSelection (int lineFrom , int indexFrom , int lineTo , int indexTo ) {
      return editor->setSelection(lineFrom, indexFrom, lineTo, indexTo);
    }

    Q_INVOKABLE void setSelectionBackgroundColor (const QColor & col ) {
      return editor->setSelectionBackgroundColor(col);
    }

    Q_INVOKABLE void setSelectionForegroundColor (const QColor & col ) {
      return editor->setSelectionForegroundColor(col);
    }

    Q_INVOKABLE void setTabIndents (bool indent ) {
      return editor->setTabIndents(indent);
    }

    Q_INVOKABLE void setTabWidth (int width ) {
      return editor->setTabWidth(width);
    }

    Q_INVOKABLE void setText (const QString & text ) {
      return editor->setText(text);
    }

    Q_INVOKABLE void setUtf8 (bool cp ) {
      return editor->setUtf8(cp);
    }

    Q_INVOKABLE void setWhitespaceVisibility (QsciScintilla::WhitespaceVisibility mode ) {
      return editor->setWhitespaceVisibility(mode);
    }

    Q_INVOKABLE void setWrapMode (QsciScintilla::WrapMode mode ) {
      return editor->setWrapMode(mode);
    }

    Q_INVOKABLE void undo () {
      return editor->undo();
    }

    Q_INVOKABLE void unindent (int line ) {
      return editor->unindent(line);
    }

    Q_INVOKABLE void zoomIn (int range ) {
      return editor->zoomIn(range);
    }

    Q_INVOKABLE void zoomIn () {
      return editor->zoomIn();
    }

    Q_INVOKABLE void zoomOut (int range ) {
      return editor->zoomOut(range);
    }

    Q_INVOKABLE void zoomOut () {
      return editor->zoomOut();
    }

    Q_INVOKABLE void zoomTo (int size ) {
      return editor->zoomTo(size);
    }

    Q_INVOKABLE QStringList apiContext (int pos , int & context_start , int & last_word_start ) {
      return editor->apiContext(pos, context_start, last_word_start);
    }

    Q_INVOKABLE void annotate (int line , const QString & text , int style ) {
      return editor->annotate(line, text, style);
    }

    Q_INVOKABLE void annotate (int line , const QString & text , const QsciStyle & style ) {
      return editor->annotate(line, text, style);
    }

    Q_INVOKABLE void annotate (int line , const QsciStyledText & text ) {
      return editor->annotate(line, text);
    }

    Q_INVOKABLE void annotate (int line , const QList<QsciStyledText> & text ) {
      return editor->annotate(line, text);
    }

    Q_INVOKABLE QString annotation (int line ) const {
      return editor->annotation(line);
    }

    //Q_INVOKABLE AnnotationDisplay annotationDisplay () const {
    //  return editor->annotationDisplay();
    //}

    Q_INVOKABLE void clearAnnotations (int line = -1 ) {
      return editor->clearAnnotations(line);
    }

    Q_INVOKABLE bool autoCompletionCaseSensitivity () const {
      return editor->autoCompletionCaseSensitivity();
    }

    Q_INVOKABLE bool autoCompletionFillupsEnabled () const {
      return editor->autoCompletionFillupsEnabled();
    }

    Q_INVOKABLE bool autoCompletionReplaceWord () const {
      return editor->autoCompletionReplaceWord();
    }

    Q_INVOKABLE bool autoCompletionShowSingle () const {
      return editor->autoCompletionShowSingle();
    }

    Q_INVOKABLE QsciScintilla::AutoCompletionSource autoCompletionSource () const {
      return editor->autoCompletionSource();
    }

    Q_INVOKABLE int autoCompletionThreshold () const {
      return editor->autoCompletionThreshold();
    }

    Q_INVOKABLE bool autoIndent () const {
      return editor->autoIndent();
    }

    Q_INVOKABLE bool backspaceUnindents () const {
      return editor->backspaceUnindents();
    }

    Q_INVOKABLE void beginUndoAction () {
      return editor->beginUndoAction();
    }

    Q_INVOKABLE QsciScintilla::BraceMatch braceMatching () const {
      return editor->braceMatching();
    }

    Q_INVOKABLE QsciScintilla::CallTipsStyle callTipsStyle () const {
      return editor->callTipsStyle();
    }

    Q_INVOKABLE int callTipsVisible () const {
      return editor->callTipsVisible();
    }

    Q_INVOKABLE void cancelList () {
      return editor->cancelList();
    }

    Q_INVOKABLE bool caseSensitive () const {
      return editor->caseSensitive();
    }

    Q_INVOKABLE void clearRegisteredImages () {
      return editor->clearRegisteredImages();
    }

    Q_INVOKABLE QColor color () const {
      return editor->color();
    }

    Q_INVOKABLE void convertEols (QsciScintilla::EolMode mode ) {
      return editor->convertEols(mode);
    }

    Q_INVOKABLE QsciDocument document () const {
      return editor->document();
    }

    Q_INVOKABLE void endUndoAction () {
      return editor->endUndoAction();
    }

    Q_INVOKABLE QColor edgeColor () const {
      return editor->edgeColor();
    }

    Q_INVOKABLE int edgeColumn () const {
      return editor->edgeColumn();
    }

    Q_INVOKABLE QsciScintilla::EdgeMode edgeMode () const {
      return editor->edgeMode();
    }

    Q_INVOKABLE void setFont (const QFont & f ) {
      return editor->setFont(f);
    }

    Q_INVOKABLE QsciScintilla::EolMode eolMode () const {
      return editor->eolMode();
    }

    Q_INVOKABLE bool eolVisibility () const {
      return editor->eolVisibility();
    }

    Q_INVOKABLE bool findFirst (const QString & expr , bool re , bool cs , bool wo , bool wrap , bool forward = true , int line = -1 , int index = -1 , bool show = true ) {
      return editor->findFirst(expr, re, cs, wo, wrap, forward, line, index, show);
    }

    Q_INVOKABLE bool findNext () {
      return editor->findNext();
    }

    Q_INVOKABLE int firstVisibleLine () const {
      return editor->firstVisibleLine();
    }

    Q_INVOKABLE QsciScintilla::FoldStyle folding () const {
      return editor->folding();
    }

    Q_INVOKABLE void getCursorPosition (int * line , int * index ) const {
      return editor->getCursorPosition(line, index);
    }

    Q_INVOKABLE void getSelection (int * lineFrom , int * indexFrom , int * lineTo , int * indexTo ) const {
      return editor->getSelection(lineFrom, indexFrom, lineTo, indexTo);
    }

    Q_INVOKABLE bool hasSelectedText () const {
      return editor->hasSelectedText();
    }

    Q_INVOKABLE int indentation (int line ) const {
      return editor->indentation(line);
    }

    Q_INVOKABLE bool indentationGuides () const {
      return editor->indentationGuides();
    }

    Q_INVOKABLE bool indentationsUseTabs () const {
      return editor->indentationsUseTabs();
    }

    Q_INVOKABLE int indentationWidth () const {
      return editor->indentationWidth();
    }

    Q_INVOKABLE bool isCallTipActive () const {
      return editor->isCallTipActive();
    }

    Q_INVOKABLE bool isListActive () const {
      return editor->isListActive();
    }

    Q_INVOKABLE bool isModified () const {
      return editor->isModified();
    }

    Q_INVOKABLE bool isReadOnly () const {
      return editor->isReadOnly();
    }

    Q_INVOKABLE bool isRedoAvailable () const {
      return editor->isRedoAvailable();
    }

    Q_INVOKABLE bool isUndoAvailable () const {
      return editor->isUndoAvailable();
    }

    Q_INVOKABLE bool isUtf8 () const {
      return editor->isUtf8();
    }

    Q_INVOKABLE bool isWordCharacter (char ch ) const {
      return editor->isWordCharacter(ch);
    }

    Q_INVOKABLE int lineAt (const QPoint & pos ) const {
      return editor->lineAt(pos);
    }

    Q_INVOKABLE void lineIndexFromPosition (int position , int * line , int * index ) const {
      return editor->lineIndexFromPosition(position, line, index);
    }

    Q_INVOKABLE int lineLength (int line ) const {
      return editor->lineLength(line);
    }

    Q_INVOKABLE int lines () const {
      return editor->lines();
    }

    Q_INVOKABLE int length () const {
      return editor->length();
    }

    Q_INVOKABLE QsciLexer * lexer () const {
      return editor->lexer();
    }

    Q_INVOKABLE bool marginLineNumbers (int margin ) const {
      return editor->marginLineNumbers(margin);
    }

    Q_INVOKABLE int marginMarkerMask (int margin ) const {
      return editor->marginMarkerMask(margin);
    }

    Q_INVOKABLE bool marginSensitivity (int margin ) const {
      return editor->marginSensitivity(margin);
    }

    Q_INVOKABLE QsciScintilla::MarginType marginType (int margin ) const {
      return editor->marginType(margin);
    }

    Q_INVOKABLE int marginWidth (int margin ) const {
      return editor->marginWidth(margin);
    }

    //Q_INVOKABLE int markerDefine (MarkerSymbol sym , int mnr = -1 ) {
    //  return editor->markerDefine(sym, mnr);
    //}

    Q_INVOKABLE int markerDefine (char ch , int mnr = -1 ) {
      return editor->markerDefine(ch, mnr);
    }

    Q_INVOKABLE int markerDefine (const QPixmap & pm , int mnr = -1 ) {
      return editor->markerDefine(pm, mnr);
    }

    Q_INVOKABLE int markerAdd (int linenr , int mnr ) {
      return editor->markerAdd(linenr, mnr);
    }

    Q_INVOKABLE unsigned markersAtLine (int linenr ) const {
      return editor->markersAtLine(linenr);
    }

    Q_INVOKABLE void markerDelete (int linenr , int mnr = -1 ) {
      return editor->markerDelete(linenr, mnr);
    }

    Q_INVOKABLE void markerDeleteAll (int mnr = -1 ) {
      return editor->markerDeleteAll(mnr);
    }

    Q_INVOKABLE void markerDeleteHandle (int mhandle ) {
      return editor->markerDeleteHandle(mhandle);
    }

    Q_INVOKABLE int markerLine (int mhandle ) const {
      return editor->markerLine(mhandle);
    }

    Q_INVOKABLE int markerFindNext (int linenr , unsigned mask ) const {
      return editor->markerFindNext(linenr, mask);
    }

    Q_INVOKABLE int markerFindPrevious (int linenr , unsigned mask ) const {
      return editor->markerFindPrevious(linenr, mask);
    }

    Q_INVOKABLE QColor paper () const {
      return editor->paper();
    }

    Q_INVOKABLE int positionFromLineIndex (int line , int index ) const {
      return editor->positionFromLineIndex(line, index);
    }

    Q_INVOKABLE bool read (QIODevice * io ) {
      return editor->read(io);
    }

    Q_INVOKABLE void recolor (int start = 0 , int end = -1 ) {
      return editor->recolor(start, end);
    }

    Q_INVOKABLE void registerImage (int id , const QPixmap & pm ) {
      return editor->registerImage(id, pm);
    }

    Q_INVOKABLE void replace (const QString & replaceStr ) {
      return editor->replace(replaceStr);
    }

    Q_INVOKABLE void resetFoldMarginColors () {
      return editor->resetFoldMarginColors();
    }

    Q_INVOKABLE void setFoldMarginColors (const QColor & fore , const QColor & back ) {
      return editor->setFoldMarginColors(fore, back);
    }

    //Q_INVOKABLE void setAnnotationDisplay (AnnotationDisplay display ) {
    //  return editor->setAnnotationDisplay(display);
    //}

    Q_INVOKABLE void setAutoCompletionFillupsEnabled (bool enabled ) {
      return editor->setAutoCompletionFillupsEnabled(enabled);
    }

    Q_INVOKABLE void setAutoCompletionFillups (const char * fillups ) {
      return editor->setAutoCompletionFillups(fillups);
    }

    Q_INVOKABLE void setAutoCompletionWordSeparators (const QStringList & separators ) {
      return editor->setAutoCompletionWordSeparators(separators);
    }

    Q_INVOKABLE void setCallTipsBackgroundColor (const QColor & col ) {
      return editor->setCallTipsBackgroundColor(col);
    }

    Q_INVOKABLE void setCallTipsForegroundColor (const QColor & col ) {
      return editor->setCallTipsForegroundColor(col);
    }

    Q_INVOKABLE void setCallTipsHighlightColor (const QColor & col ) {
      return editor->setCallTipsHighlightColor(col);
    }

    Q_INVOKABLE void setCallTipsStyle (QsciScintilla::CallTipsStyle style ) {
      return editor->setCallTipsStyle(style);
    }

    Q_INVOKABLE void setCallTipsVisible (int nr ) {
      return editor->setCallTipsVisible(nr);
    }

    Q_INVOKABLE void setDocument (const QsciDocument & document ) {
      return editor->setDocument(document);
    }

    Q_INVOKABLE void setEdgeColor (const QColor & col ) {
      return editor->setEdgeColor(col);
    }

    Q_INVOKABLE void setEdgeColumn (int colnr ) {
      return editor->setEdgeColumn(colnr);
    }

    Q_INVOKABLE void setEdgeMode (QsciScintilla::EdgeMode mode ) {
      return editor->setEdgeMode(mode);
    }

    Q_INVOKABLE void setMarginText (int line , const QString & text , int style ) {
      return editor->setMarginText(line, text, style);
    }

    Q_INVOKABLE void setMarginText (int line , const QString & text , const QsciStyle & style ) {
      return editor->setMarginText(line, text, style);
    }

    Q_INVOKABLE void setMarginText (int line , const QsciStyledText & text ) {
      return editor->setMarginText(line, text);
    }

    //Q_INVOKABLE void setMarginText (int line , const QList<QsciStyledText> & text ) {
    //  return editor->setMarginText(line, text);
    //}

    Q_INVOKABLE void setMarginType (int margin , QsciScintilla::MarginType type ) {
      return editor->setMarginType(margin, type);
    }

    Q_INVOKABLE void clearMarginText (int line = -1 ) {
      return editor->clearMarginText(line);
    }

    Q_INVOKABLE void setMarkerBackgroundColor (const QColor & col , int mnr = -1 ) {
      return editor->setMarkerBackgroundColor(col, mnr);
    }

    Q_INVOKABLE void setMarkerForegroundColor (const QColor & col , int mnr = -1 ) {
      return editor->setMarkerForegroundColor(col, mnr);
    }

    Q_INVOKABLE void setMatchedBraceBackgroundColor (const QColor & col ) {
      return editor->setMatchedBraceBackgroundColor(col);
    }

    Q_INVOKABLE void setMatchedBraceForegroundColor (const QColor & col ) {
      return editor->setMatchedBraceForegroundColor(col);
    }

    Q_INVOKABLE void setUnmatchedBraceBackgroundColor (const QColor & col ) {
      return editor->setUnmatchedBraceBackgroundColor(col);
    }

    Q_INVOKABLE void setUnmatchedBraceForegroundColor (const QColor & col ) {
      return editor->setUnmatchedBraceForegroundColor(col);
    }

    //Q_INVOKABLE void setWrapVisualFlags (QsciScintilla::WrapVisualFlag eflag , QsciScintilla::WrapVisualFlag sflag = QsciScintilla::WrapVisualFlag::WrapFlagNone , int sindent = 0 ) {
    //  return editor->setWrapVisualFlags(eflag, sflag, sindent);
    //}

    Q_INVOKABLE QString selectedText () const {
      return editor->selectedText();
    }

    Q_INVOKABLE bool selectionToEol () const {
      return editor->selectionToEol();
    }

    Q_INVOKABLE void setSelectionToEol (bool filled ) {
      return editor->setSelectionToEol(filled);
    }

    Q_INVOKABLE void showUserList (int id , const QStringList & list ) {
      return editor->showUserList(id, list);
    }

    Q_INVOKABLE QsciCommandSet * standardCommands () const {
      return editor->standardCommands();
    }

    Q_INVOKABLE bool tabIndents () const {
      return editor->tabIndents();
    }

    Q_INVOKABLE int tabWidth () const {
      return editor->tabWidth();
    }

    Q_INVOKABLE QString text () const {
      return editor->text();
    }

    Q_INVOKABLE QString text (int line ) const {
      return editor->text(line);
    }

    Q_INVOKABLE int textHeight (int linenr ) const {
      return editor->textHeight(linenr);
    }

    Q_INVOKABLE QsciScintilla::WhitespaceVisibility whitespaceVisibility () const {
      return editor->whitespaceVisibility();
    }

    Q_INVOKABLE QString wordAtPoint (const QPoint & point ) const {
      return editor->wordAtPoint(point);
    }

    Q_INVOKABLE const char * wordCharacters () const {
      return editor->wordCharacters();
    }

    Q_INVOKABLE QsciScintilla::WrapMode wrapMode () const {
      return editor->wrapMode();
    }

    Q_INVOKABLE bool write (QIODevice * io ) const {
      return editor->write(io);
    }

};

#endif //QSCITE_EDITOR_SCRIPT_WRAPPER_H_