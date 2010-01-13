#ifndef _QSCITE_PREFS_H
#define _QSCITE_PREFS_H

#include "ui_dlgPrefsUI.h"

#include <QDialog>
#include <QSettings>
#include "qsciteeditor.h"
class QTerminal;

class MainPrefsDialog: public QDialog, protected Ui_QSciTEPrefs {
	
	Q_OBJECT
	
public:
	MainPrefsDialog(QWidget * parent=0, Qt::WindowFlags f=0);
	
signals:
	void prefsWereReset();
	
private slots:
	void lexerSelected(const QString & lexer);
	
	void addExt();
	void delExt();
	void saveExts();
	
	void addMagic();
	void delMagic();
	void saveMagic();
	
	void resetToDefaults();
	void updateSampleDoc();
	void updateSampleTerm();
	void writeValues();
	
private:
	void populate();

	QSettings settings;
	
};

void writeDefaultSettings(QSettings & settings);

void applySettingsToDoc(QsciteEditor * curDoc);
void applyPrefsToTerminal(QTerminal * term);

#endif
