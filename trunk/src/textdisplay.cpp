#include "textdisplay.h"
#include "mainwindow.h"
#include "lexer_utils.h"
#include "prefs.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

TextDisplayPanel::TextDisplayPanel(QWidget * _parent, Qt::WindowFlags f) :
	QWidget(_parent, f), parent(dynamic_cast<MainWindow *>(_parent))
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	setupUi(this);
	btnFont->setDefaultAction(parent->fontAct);
	
	cbxLexer->blockSignals(true);
	cbxLexer->addItem("None (plain text)");
	for (int i = 0; !supportedLexers[i].isEmpty(); ++i) {
		cbxLexer->addItem(supportedLexers[i]);
	}
	
	populate();
	cbxLexer->blockSignals(false);
	
	connect(sbFgR, SIGNAL(valueChanged(int)), this, SLOT(applyColor()));
	connect(sbFgG, SIGNAL(valueChanged(int)), this, SLOT(applyColor()));
	connect(sbFgB, SIGNAL(valueChanged(int)), this, SLOT(applyColor()));
	connect(sbBgR, SIGNAL(valueChanged(int)), this, SLOT(applyColor()));
	connect(sbBgG, SIGNAL(valueChanged(int)), this, SLOT(applyColor()));
	connect(sbBgB, SIGNAL(valueChanged(int)), this, SLOT(applyColor()));
}

namespace {
	/* Read back the wrap visual information from a QsciScintilla.
	 * This method is oddly missing from the QsciScintilla class. Would be
	 * a good use for an Objective-C category, but this is C++.
	 */
	QsciScintilla::WrapVisualFlag getWrapVisualFlag(QsciScintilla * e) {
		int showFlags = e->SendScintilla( QsciScintillaBase::SCI_GETWRAPVISUALFLAGS );
		int flagsNearText = e->SendScintilla( QsciScintillaBase::SCI_GETWRAPVISUALFLAGSLOCATION);
		return showFlags ? (flagsNearText ? QsciScintilla::WrapFlagByText : QsciScintilla::WrapFlagByBorder) : QsciScintilla::WrapFlagNone;
	}
}

void TextDisplayPanel::populate() {
	if (parent->openFiles.empty()) {
		this->setEnabled(false);
		return;
	}
	this->setEnabled(true);
	
	QsciScintilla * theEditor = parent->openFiles[parent->curDocIdx].edWidget;
	cbWrapMode->setChecked(theEditor->wrapMode() == QsciScintilla::WrapWord);
	cbxWrapMarkers->setCurrentIndex(getWrapVisualFlag(theEditor));
	cbLineNos->setChecked(theEditor->marginWidth(1) > 0);
	cbIndentGuides->setChecked(theEditor->indentationGuides());
	sbIndentSize->setValue(theEditor->tabWidth());
	cbAutoIndent->setChecked(theEditor->autoIndent());
	cbxUseTabs->setCurrentIndex(theEditor->indentationsUseTabs() ? 1 : 0);
	cbxEOLMode->setCurrentIndex(theEditor->eolMode());
	cbxLexer->blockSignals(true);
	if (theEditor->lexer() == NULL) {
		cbxLexer->setCurrentIndex(0);
	} else {
		cbxLexer->setCurrentIndex(indexOfLexer(theEditor->lexer()->lexer()) + 1);
	}
	cbxLexer->blockSignals(false);
	QColor fore = theEditor->color();
	QColor back = theEditor->paper();
	sbFgR->setValue(fore.red());
	sbFgG->setValue(fore.green());
	sbFgB->setValue(fore.blue());
	sbBgR->setValue(back.red());
	sbBgG->setValue(back.green());
	sbBgB->setValue(back.blue());
}

void TextDisplayPanel::on_btnReset_clicked() {
	if (parent->openFiles.empty()) {
		return;
	}
	applySettingsToDoc(parent->openFiles[parent->curDocIdx].edWidget);
	populate();
}

void TextDisplayPanel::on_btnSaveSettings_clicked() {
	QSettings settings;
	settings.setValue("wrapMode", cbWrapMode->isChecked() ? QsciScintilla::WrapWord : QsciScintilla::WrapNone);
	settings.setValue("wrapIndicatorMode", cbxWrapMarkers->currentIndex());
	
	settings.setValue("showLineNumbers", cbLineNos->isChecked());
	
	settings.setValue("indentGuides", cbIndentGuides->isChecked());
	settings.setValue("indentUseTabs", (bool)(cbxUseTabs->currentIndex()));
	settings.setValue("indentWidth", sbIndentSize->value());
	settings.setValue("autoIndent", cbAutoIndent->isChecked());
	settings.setValue("EOLMode", cbxEOLMode->currentIndex());
	
	settings.beginGroup("font");
	if (!parent->openFiles.empty()) {
		QsciScintilla * theEditor = parent->openFiles[parent->curDocIdx].edWidget;
		settings.setValue("docFamily", theEditor->font().family());
		settings.setValue("docSize", theEditor->font().pointSize());
	}
	settings.setValue(
		"docColorFg",
		QColor(sbFgR->value(), sbFgG->value(), sbFgB->value())
	);
	settings.setValue(
		"docColorBg",
		QColor(sbBgR->value(), sbBgG->value(), sbBgB->value())
	);
	settings.endGroup();
}

void TextDisplayPanel::on_cbWrapMode_clicked(bool checked) {
	parent->openFiles[parent->curDocIdx].edWidget->setWrapMode(checked ? QsciScintilla::WrapWord : QsciScintilla::WrapNone);
}

void TextDisplayPanel::on_cbxWrapMarkers_currentIndexChanged(int idx) {
	parent->openFiles[parent->curDocIdx].edWidget->setWrapVisualFlags(static_cast<QsciScintilla::WrapVisualFlag>(idx));
}

void TextDisplayPanel::on_cbLineNos_clicked(bool checked) {
	if (checked) {
		parent->openFiles[parent->curDocIdx].edWidget->setMarginWidth(1, "9999");
		parent->redoSetMargin();
	} else {
		parent->openFiles[parent->curDocIdx].edWidget->setMarginWidth(1, 0);
	}
}

void TextDisplayPanel::on_cbIndentGuides_clicked(bool checked) {
	parent->openFiles[parent->curDocIdx].edWidget->setIndentationGuides(checked);
}

void TextDisplayPanel::on_sbIndentSize_valueChanged(int val) {
	parent->openFiles[parent->curDocIdx].edWidget->setIndentationWidth(val);
	parent->openFiles[parent->curDocIdx].edWidget->setTabWidth(val);
}

void TextDisplayPanel::on_cbAutoIndent_clicked(bool checked) {
	parent->openFiles[parent->curDocIdx].edWidget->setAutoIndent(checked);
}

void TextDisplayPanel::on_cbxUseTabs_currentIndexChanged(int idx) {
	parent->openFiles[parent->curDocIdx].edWidget->setIndentationsUseTabs(idx == 1);
}

void TextDisplayPanel::on_cbxEOLMode_currentIndexChanged(int idx) {
	parent->openFiles[parent->curDocIdx].edWidget->setEolMode( static_cast<QsciScintilla::EolMode>(idx) );
}

void TextDisplayPanel::on_cbxLexer_currentIndexChanged(int idx) {
	QsciLexer * newLexer = NULL;
	if (idx > 0) {
		newLexer = getLexerByName(supportedLexers[idx - 1]);
		newLexer->setParent(parent->openFiles[parent->curDocIdx].edWidget);
		QFont curFont = parent->openFiles[parent->curDocIdx].edWidget->font();
		setLexerFont(newLexer, curFont.family(), curFont.pointSize());
	}
	parent->openFiles[parent->curDocIdx].edWidget->setLexer(newLexer);
}

void TextDisplayPanel::applyColor() {
	parent->openFiles[parent->curDocIdx].edWidget->setColor( QColor(sbFgR->value(), sbFgG->value(), sbFgB->value()) );
	parent->openFiles[parent->curDocIdx].edWidget->setPaper( QColor(sbBgR->value(), sbBgG->value(), sbBgB->value()) );
}
