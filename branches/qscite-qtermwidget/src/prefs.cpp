#include <QtGui>
#include <QtDebug>
#include "qsciteeditor.h"

#include "prefs.h"
#include "lexer_utils.h"

#ifdef _WIN32
  #include "qterminal.h"
#else
  #include "qterminal_pty.h"
#endif

MainPrefsDialog::MainPrefsDialog(QWidget * parent, Qt::WindowFlags f) :
	QDialog(parent, f), settings()
{
	setupUi(this);
#ifndef Q_WS_MAC
	cbTermDrawer->hide();
#endif
	
	connect(btnDefaults, SIGNAL(clicked()), this, SLOT(resetToDefaults()));
	
	connect(this, SIGNAL(accepted()), this, SLOT(writeValues()));
	
	connect(lwTypes, SIGNAL(currentTextChanged(const QString &)),
	        this,    SLOT(lexerSelected(const QString&)));
	        
	connect(btnAddExt, SIGNAL(clicked()), this, SLOT(addExt()));
	connect(btnDelExt, SIGNAL(clicked()), this, SLOT(delExt()));
	
	connect(btnAddMagic, SIGNAL(clicked()), this, SLOT(addMagic()));
	connect(btnDelMagic, SIGNAL(clicked()), this, SLOT(delMagic()));
	
	connect(cbxTextFont, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbPtSize, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbFgR, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbFgG, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbFgB, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbBgR, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbBgG, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));
	connect(sbBgB, SIGNAL(valueChanged(int)), this, SLOT(updateSampleDoc()));

	connect(cbxTermFont, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermSize, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermFgR, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermFgG, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermFgB, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermBgR, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermBgG, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	connect(sbTermBgB, SIGNAL(valueChanged(int)), this, SLOT(updateSampleTerm()));
	
	populate();
}

/*
 * Set GUI controls to match saved settings
 */
void MainPrefsDialog::populate() {
	/*
	 * "General" tab
	 */
	cbSaveSize->setChecked(settings.value("saveWindowGeometry").toBool());
	sbRecentCount->setValue(settings.value("recentFileCount").toInt());
	cbWrapMode->setChecked(
		settings.value("wrapMode").toInt() == QsciteEditor::WrapWord
	);
	cbxWrapMarkers->setCurrentIndex(
		settings.value("wrapIndicatorMode").toInt()
	);
	cbLineNos->setChecked(settings.value("showLineNumbers").toBool());
	cbIndentGuides->setChecked(settings.value("indentGuides").toBool());
	cbxUseTabs->setCurrentIndex(
		settings.value("indentUseTabs").toBool() ? 1 : 0
	);
	sbIndentSize->setValue(settings.value("indentWidth").toInt());
	cbAutoIndent->setChecked(settings.value("autoIndent").toBool());
	cbxEOLMode->setCurrentIndex(settings.value("EOLMode").toInt());
	
	cbTrayIcon->setChecked(settings.value("trayIcon", true).toBool());
	sbOpacity->setValue(settings.value("wndOpacity", 1.0).toDouble());
	
	/*
	 * "File types" tab
	 */
	lwTypes->clear();
	for (int i = 0; !supportedLexers[i].isEmpty(); ++i) {
		lwTypes->addItem(supportedLexers[i]);
	}
	// We will handle the extensions and magic when a type is selected
	
	/*
	 * "Fonts" tab
	 */
	settings.beginGroup("font");
	cbxTextFont->setCurrentFont(
		QFont( settings.value("docFamily").toString() )
	);
	sbPtSize->setValue(settings.value("docSize").toInt());
	QColor docFore = settings.value("docColorFg").value<QColor>();
	sbFgR->setValue(docFore.red());
	sbFgG->setValue(docFore.green());
	sbFgB->setValue(docFore.blue());
	QColor docBack = settings.value("docColorBg").value<QColor>();
	sbBgR->setValue(docBack.red());
	sbBgG->setValue(docBack.green());
	sbBgB->setValue(docBack.blue());
	settings.endGroup();
	
	/*
	 * "Terminal" tab
	 */
#ifdef Q_WS_MAC
	cbTermDrawer->setChecked(settings.value("terminalInDrawer").toBool());
#endif
	settings.beginGroup("font");
	cbxTermFont->setCurrentFont(
		QFont( settings.value("termFamily").toString() )
	);
	sbTermSize->setValue(settings.value("termSize").toInt());
	QColor termFore = settings.value("termColorFg").value<QColor>();
	sbTermFgR->setValue(termFore.red());
	sbTermFgG->setValue(termFore.green());
	sbTermFgB->setValue(termFore.blue());
	QColor termBack = settings.value("termColorBg").value<QColor>();
	sbTermBgR->setValue(termBack.red());
	sbTermBgG->setValue(termBack.green());
	sbTermBgB->setValue(termBack.blue());
	settings.endGroup();
}

void MainPrefsDialog::lexerSelected(const QString & lexer) {
	/* Don't signal in response to our own changes */
	lwExtns->disconnect(SIGNAL(itemChanged(QListWidgetItem *)));
	lwMagic->disconnect(SIGNAL(itemChanged(QListWidgetItem *)));
	
	lwExtns->clear();
	settings.beginGroup("extensions");
	for (int i = 0, j = settings.beginReadArray(lexer); i < j; ++i) {
		settings.setArrayIndex(i);
		(new QListWidgetItem(settings.value("ext").toString(), lwExtns))->setFlags((Qt::ItemFlags)35);
	}
	settings.endArray();
	settings.endGroup();
	
	lwMagic->clear();
	settings.beginGroup("magic");
	for (int i = 0, j = settings.beginReadArray(lexer); i < j; ++i) {
		settings.setArrayIndex(i);
		(new QListWidgetItem(settings.value("str").toString(), lwMagic))->setFlags((Qt::ItemFlags)35);
	}
	settings.endArray();
	settings.endGroup();
	
	// Now we can signal.
	connect(lwExtns, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(saveExts()));
	connect(lwMagic, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(saveMagic()));
}

void MainPrefsDialog::addExt() {
	QListWidgetItem * newItem = new QListWidgetItem("<new>", lwExtns);
	newItem->setFlags((Qt::ItemFlags)35);
	lwExtns->setFocus();
	lwExtns->editItem(newItem);
}

void MainPrefsDialog::delExt() {
	delete lwExtns->takeItem(lwExtns->currentRow());
	saveExts();
}

void MainPrefsDialog::saveExts() {
	settings.beginGroup("extensions");
	settings.remove(lwTypes->currentItem()->text());
	settings.beginWriteArray(lwTypes->currentItem()->text(), lwExtns->count());
	for (int i = 0; i < lwExtns->count(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("ext", lwExtns->item(i)->text());
	}
	settings.endArray();
	settings.endGroup();
}

void MainPrefsDialog::addMagic() {
	QListWidgetItem * newItem = new QListWidgetItem("<new>", lwMagic);
	newItem->setFlags((Qt::ItemFlags)35);
	lwMagic->setFocus();
	lwMagic->editItem(newItem);
}

void MainPrefsDialog::delMagic() {
	delete lwMagic->takeItem(lwMagic->currentRow());
	saveMagic();
}

void MainPrefsDialog::saveMagic() {
	settings.beginGroup("magic");
	settings.remove(lwTypes->currentItem()->text());
	settings.beginWriteArray(lwTypes->currentItem()->text(), lwMagic->count());
	for (int i = 0; i < lwMagic->count(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("str", lwMagic->item(i)->text());
	}
	settings.endArray();
	settings.endGroup();
}

void MainPrefsDialog::updateSampleDoc() {
	txtPreview->setFont(QFont(cbxTextFont->currentText(), sbPtSize->value()));
	QPalette p = txtPreview->palette();
	p.setColor(QPalette::Text, QColor(sbFgR->value(), sbFgG->value(), sbFgB->value()));
	p.setColor(QPalette::Base, QColor(sbBgR->value(), sbBgG->value(), sbBgB->value()));
	txtPreview->setPalette(p);
}

void MainPrefsDialog::updateSampleTerm() {
	txtTermPreview->setFont(QFont(cbxTermFont->currentText(), sbTermSize->value()));
	QPalette p = txtTermPreview->palette();
	p.setColor(QPalette::Text, QColor(sbTermFgR->value(), sbTermFgG->value(), sbTermFgB->value()));
	p.setColor(QPalette::Base, QColor(sbTermBgR->value(), sbTermBgG->value(), sbTermBgB->value()));
	txtTermPreview->setPalette(p);
}

void MainPrefsDialog::resetToDefaults() {
	writeDefaultSettings(settings);
	populate();
	emit prefsWereReset();
}

void MainPrefsDialog::writeValues() {
	/*
	 * "General" tab
	 */
	settings.setValue("saveWindowGeometry", cbSaveSize->isChecked());
	settings.setValue("recentFileCount", sbRecentCount->value());

	settings.setValue("wrapMode", cbWrapMode->isChecked() ? QsciteEditor::WrapWord : QsciteEditor::WrapNone);
	settings.setValue("wrapIndicatorMode", cbxWrapMarkers->currentIndex());
	
	settings.setValue("showLineNumbers", cbLineNos->isChecked());
	
	settings.setValue("indentGuides", cbIndentGuides->isChecked());
	settings.setValue("indentUseTabs", (bool)(cbxUseTabs->currentIndex()));
	settings.setValue("indentWidth", sbIndentSize->value());
	settings.setValue("autoIndent", cbAutoIndent->isChecked());
	settings.setValue("EOLMode", cbxEOLMode->currentIndex());
	
	settings.setValue("trayIcon", cbTrayIcon->isChecked());
	settings.setValue("wndOpacity", sbOpacity->value());
	
	/*
	 * "File types" tab
	 */
	/*
	 * File type information is saved immediately.
	 */
	/*
	 * "Fonts" tab
	 */
	settings.beginGroup("font");
	settings.setValue("docFamily", cbxTextFont->currentFont().family());
	settings.setValue("docSize", sbPtSize->value());
	settings.setValue(
		"docColorFg",
		QColor(sbFgR->value(), sbFgG->value(), sbFgB->value())
	);
	settings.setValue(
		"docColorBg",
		QColor(sbBgR->value(), sbBgG->value(), sbBgB->value())
	);
	settings.endGroup();
	
	/*
	 * "Terminal" tab
	 */
#ifdef Q_WS_MAC
	settings.setValue("terminalInDrawer", cbTermDrawer->isChecked());
#endif
	settings.beginGroup("font");
	settings.setValue("termFamily", cbxTermFont->currentFont().family());
	settings.setValue("termSize", sbTermSize->value());
	settings.setValue(
		"termColorFg",
		QColor(sbTermFgR->value(), sbTermFgG->value(), sbTermFgB->value())
	);
	settings.setValue(
		"termColorBg",
		QColor(sbTermBgR->value(), sbTermBgG->value(), sbTermBgB->value())
	);
	settings.endGroup();
}

void writeDefaultSettings(QSettings & settings) {
	settings.remove("");

	settings.beginGroup("extensions");
	writeDefaultExtensions(settings);
	settings.endGroup();

	settings.beginGroup("magic");
	writeDefaultMagic(settings);
	settings.endGroup();

#ifdef Q_WS_MAC
	settings.setValue("terminalInDrawer", false);
#endif

    settings.setValue("recentFileCount", 10);
	settings.setValue("saveWindowGeometry", true);
	
	settings.setValue("trayIcon", true);
	settings.setValue("wndOpacity", 1.0);

	settings.setValue("EOLMode", QsciteEditor::EolUnix);
	settings.setValue("wrapMode", QsciteEditor::WrapWord);
	settings.setValue("wrapIndicatorMode", QsciteEditor::WrapFlagNone);
	settings.setValue("showLineNumbers", true);
	
	settings.setValue("indentUseTabs", false);
	settings.setValue("indentWidth", 2);
	settings.setValue("indentGuides", true);
	settings.setValue("autoIndent", true);
	
	settings.beginGroup("font");
	settings.setValue("docFamily", QSCITE_MONO_FAMILY);
	settings.setValue("docSize", 10);
	settings.setValue("docColorFg", QColor(0,0,0));
	settings.setValue("docColorBg", QColor(255, 255, 255));
	
	settings.setValue("termFamily", QSCITE_MONO_FAMILY);
	settings.setValue("termSize", 10);
	settings.setValue("termColorFg", QColor(0,0,0));
	settings.setValue("termColorBg", QColor(255, 255, 255));
	settings.endGroup();
	
	settings.setValue("version", 1);
}

void applySettingsToDoc(QsciteEditor * curDoc) {
	QSettings settings;

	// Default EOL mode to LF
	curDoc->setEolMode(
		static_cast<QsciteEditor::EolMode>(
			settings.value("EOLMode", QsciteEditor::EolUnix).toInt()
		)
	);
	
	// Default wrap mode to WrapWord
	curDoc->setWrapMode(
		static_cast<QsciteEditor::WrapMode>(
			settings.value("wrapMode").toInt()
		)
	);
	
	// Turn on line numbers by default
	if (settings.value("showLineNumbers").toBool()) {
		curDoc->setMarginLineNumbers(1, true);
		// set default margin width to 4 characters (will adjust with different files)
    int numLines = curDoc->lines();
    QString exLength = "9999";
    numLines /= 1000;
  
    while (numLines >= 1) {
      exLength += "9";
      numLines /= 10;
    }
		curDoc->setMarginWidth(1, exLength);
	} else {
		curDoc->setMarginWidth(1, 0);
	}
	
	// Don't use tab characters for indents
	curDoc->setIndentationsUseTabs(settings.value("indentUseTabs").toBool());
	
	// Default to using two spaces for each indent
	curDoc->setIndentationWidth(settings.value("indentWidth").toInt());
	curDoc->setTabWidth(settings.value("indentWidth").toInt());
	
	// Indent guides on by default
	curDoc->setIndentationGuides(settings.value("indentGuides").toBool());
	
	// Auto indent by default
	curDoc->setAutoIndent(settings.value("autoIndent").toBool());
	
	// Set wrap visual indication
	curDoc->setWrapVisualFlags(
		static_cast<QsciteEditor::WrapVisualFlag>(
			settings.value("wrapIndicatorMode").toInt()
		),
		QsciteEditor::WrapFlagNone,
		curDoc->indentationWidth()
	);
			
	// Make tabs indent and backspaces unindent
	curDoc->setTabIndents(true);
	curDoc->setBackspaceUnindents(true);
	
	// Turn on strict brace matching by default
	curDoc->setBraceMatching(
		static_cast<QsciteEditor::BraceMatch>(
			settings.value("braceMatchMode", QsciteEditor::StrictBraceMatch).toInt()
		)
	);
	// use Monospaced font at size 10 by default
	QFont baseFont(
		settings.value("font/docFamily").toString(),
		settings.value("font/docSize").toInt()
	);

	curDoc->setFont(baseFont);
	if (curDoc->lexer() != NULL) {
		setLexerFont(curDoc->lexer(), baseFont.family(), baseFont.pointSize());
	}
	
	QFontInfo test(baseFont);
	qDebug() << "Current font family: " << test.family();
	qDebug() << "Exact font match: " << (test.exactMatch() ? "true" : "false");
	
	curDoc->setColor(settings.value("font/docColorFg").value<QColor>());
	curDoc->setCaretForegroundColor(curDoc->color());
	curDoc->setPaper(settings.value("font/docColorBg").value<QColor>());
}

void applyPrefsToTerminal(QTerminal * term) {
	QSettings settings;
	term->setFont(
		QFont(
			settings.value("font/termFamily").toString(),
			settings.value("font/termSize").toInt()
		)
	);
	term->setTabStopWidth(QFontMetrics(term->font()).width("        "));
	QPalette p = term->palette();
	p.setColor(
		QPalette::Base,
		settings.value("font/termColorBg").value<QColor>()
	);
	p.setColor(
		QPalette::Text,
		settings.value("font/termColorFg").value<QColor>()
	);
	term->setPalette(p);
}

