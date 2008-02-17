/********************************************************************************
** Form generated from reading ui file 'dlgPrefsUI.ui'
**
** Created: Sat Feb 16 16:06:55 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DLGPREFSUI_H
#define UI_DLGPREFSUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_QSciTEPrefs
{
public:
    QWidget *widget;
    QHBoxLayout *hboxLayout;
    QPushButton *btnDefaults;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabs;
    QWidget *tabGeneral;
    QCheckBox *cbSaveSize;
    QGroupBox *gbDocMode;
    QWidget *layoutWidget;
    QVBoxLayout *vboxLayout;
    QCheckBox *cbWrapMode;
    QHBoxLayout *hboxLayout1;
    QLabel *lblWrapMarkers;
    QComboBox *cbxWrapMarkers;
    QCheckBox *cbLineNos;
    QHBoxLayout *hboxLayout2;
    QLabel *lblUseTabs;
    QComboBox *cbxUseTabs;
    QHBoxLayout *hboxLayout3;
    QLabel *lblIndentSize;
    QSpinBox *sbIndentSize;
    QWidget *tabFileTypes;
    QWidget *widget1;
    QGridLayout *gridLayout;
    QLabel *lblTypes;
    QLabel *lblExtns;
    QLabel *lblMagic;
    QListWidget *lwTypes;
    QListWidget *lwExtns;
    QListWidget *lwMagic;
    QHBoxLayout *hboxLayout4;
    QToolButton *btnAddExt;
    QToolButton *btnDelExt;
    QHBoxLayout *hboxLayout5;
    QToolButton *btnAddMagic;
    QToolButton *btnDelMagic;
    QWidget *tabFonts;
    QFontComboBox *fontComboBox;
    QLabel *lblFont;
    QGroupBox *gbColor;
    QWidget *widget2;
    QGridLayout *gridLayout1;
    QLabel *lblFore;
    QSpinBox *sbFgR;
    QSpinBox *sbFgG;
    QSpinBox *sbFgB;
    QLabel *lblBack;
    QSpinBox *sbBgR;
    QSpinBox *sbBgG;
    QSpinBox *sbBgB;
    QLineEdit *txtPreview;
    QWidget *widget3;
    QHBoxLayout *hboxLayout6;
    QLabel *lblPtSize;
    QSpinBox *sbPtSize;
    QSlider *slPtSize;

    void setupUi(QDialog *QSciTEPrefs)
    {
    if (QSciTEPrefs->objectName().isEmpty())
        QSciTEPrefs->setObjectName(QString::fromUtf8("QSciTEPrefs"));
    QSciTEPrefs->setWindowModality(Qt::WindowModal);
    QSciTEPrefs->resize(400, 320);
    widget = new QWidget(QSciTEPrefs);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(10, 280, 381, 32));
    hboxLayout = new QHBoxLayout(widget);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    btnDefaults = new QPushButton(widget);
    btnDefaults->setObjectName(QString::fromUtf8("btnDefaults"));

    hboxLayout->addWidget(btnDefaults);

    spacerItem = new QSpacerItem(40, 16, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    buttonBox = new QDialogButtonBox(widget);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    hboxLayout->addWidget(buttonBox);

    tabs = new QTabWidget(QSciTEPrefs);
    tabs->setObjectName(QString::fromUtf8("tabs"));
    tabs->setGeometry(QRect(10, 10, 380, 260));
    tabs->setTabShape(QTabWidget::Rounded);
    tabGeneral = new QWidget();
    tabGeneral->setObjectName(QString::fromUtf8("tabGeneral"));
    cbSaveSize = new QCheckBox(tabGeneral);
    cbSaveSize->setObjectName(QString::fromUtf8("cbSaveSize"));
    cbSaveSize->setGeometry(QRect(10, 0, 240, 21));
    gbDocMode = new QGroupBox(tabGeneral);
    gbDocMode->setObjectName(QString::fromUtf8("gbDocMode"));
    gbDocMode->setGeometry(QRect(0, 20, 370, 210));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(gbDocMode->sizePolicy().hasHeightForWidth());
    gbDocMode->setSizePolicy(sizePolicy);
    gbDocMode->setFlat(false);
    layoutWidget = new QWidget(gbDocMode);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 30, 247, 153));
    vboxLayout = new QVBoxLayout(layoutWidget);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    cbWrapMode = new QCheckBox(layoutWidget);
    cbWrapMode->setObjectName(QString::fromUtf8("cbWrapMode"));

    vboxLayout->addWidget(cbWrapMode);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    lblWrapMarkers = new QLabel(layoutWidget);
    lblWrapMarkers->setObjectName(QString::fromUtf8("lblWrapMarkers"));

    hboxLayout1->addWidget(lblWrapMarkers);

    cbxWrapMarkers = new QComboBox(layoutWidget);
    cbxWrapMarkers->setObjectName(QString::fromUtf8("cbxWrapMarkers"));

    hboxLayout1->addWidget(cbxWrapMarkers);


    vboxLayout->addLayout(hboxLayout1);

    cbLineNos = new QCheckBox(layoutWidget);
    cbLineNos->setObjectName(QString::fromUtf8("cbLineNos"));

    vboxLayout->addWidget(cbLineNos);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    lblUseTabs = new QLabel(layoutWidget);
    lblUseTabs->setObjectName(QString::fromUtf8("lblUseTabs"));

    hboxLayout2->addWidget(lblUseTabs);

    cbxUseTabs = new QComboBox(layoutWidget);
    cbxUseTabs->setObjectName(QString::fromUtf8("cbxUseTabs"));

    hboxLayout2->addWidget(cbxUseTabs);


    vboxLayout->addLayout(hboxLayout2);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    lblIndentSize = new QLabel(layoutWidget);
    lblIndentSize->setObjectName(QString::fromUtf8("lblIndentSize"));

    hboxLayout3->addWidget(lblIndentSize);

    sbIndentSize = new QSpinBox(layoutWidget);
    sbIndentSize->setObjectName(QString::fromUtf8("sbIndentSize"));
    sbIndentSize->setMinimum(1);

    hboxLayout3->addWidget(sbIndentSize);


    vboxLayout->addLayout(hboxLayout3);

    tabs->addTab(tabGeneral, QString());
    tabFileTypes = new QWidget();
    tabFileTypes->setObjectName(QString::fromUtf8("tabFileTypes"));
    widget1 = new QWidget(tabFileTypes);
    widget1->setObjectName(QString::fromUtf8("widget1"));
    widget1->setGeometry(QRect(0, 0, 371, 231));
    gridLayout = new QGridLayout(widget1);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    lblTypes = new QLabel(widget1);
    lblTypes->setObjectName(QString::fromUtf8("lblTypes"));

    gridLayout->addWidget(lblTypes, 0, 0, 1, 1);

    lblExtns = new QLabel(widget1);
    lblExtns->setObjectName(QString::fromUtf8("lblExtns"));

    gridLayout->addWidget(lblExtns, 0, 1, 1, 1);

    lblMagic = new QLabel(widget1);
    lblMagic->setObjectName(QString::fromUtf8("lblMagic"));

    gridLayout->addWidget(lblMagic, 0, 2, 1, 1);

    lwTypes = new QListWidget(widget1);
    lwTypes->setObjectName(QString::fromUtf8("lwTypes"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(lwTypes->sizePolicy().hasHeightForWidth());
    lwTypes->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(lwTypes, 1, 0, 2, 1);

    lwExtns = new QListWidget(widget1);
    lwExtns->setObjectName(QString::fromUtf8("lwExtns"));
    sizePolicy1.setHeightForWidth(lwExtns->sizePolicy().hasHeightForWidth());
    lwExtns->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(lwExtns, 1, 1, 1, 1);

    lwMagic = new QListWidget(widget1);
    lwMagic->setObjectName(QString::fromUtf8("lwMagic"));
    sizePolicy1.setHeightForWidth(lwMagic->sizePolicy().hasHeightForWidth());
    lwMagic->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(lwMagic, 1, 2, 1, 1);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    btnAddExt = new QToolButton(widget1);
    btnAddExt->setObjectName(QString::fromUtf8("btnAddExt"));

    hboxLayout4->addWidget(btnAddExt);

    btnDelExt = new QToolButton(widget1);
    btnDelExt->setObjectName(QString::fromUtf8("btnDelExt"));

    hboxLayout4->addWidget(btnDelExt);


    gridLayout->addLayout(hboxLayout4, 2, 1, 1, 1);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    btnAddMagic = new QToolButton(widget1);
    btnAddMagic->setObjectName(QString::fromUtf8("btnAddMagic"));

    hboxLayout5->addWidget(btnAddMagic);

    btnDelMagic = new QToolButton(widget1);
    btnDelMagic->setObjectName(QString::fromUtf8("btnDelMagic"));

    hboxLayout5->addWidget(btnDelMagic);


    gridLayout->addLayout(hboxLayout5, 2, 2, 1, 1);

    tabs->addTab(tabFileTypes, QString());
    tabFonts = new QWidget();
    tabFonts->setObjectName(QString::fromUtf8("tabFonts"));
    fontComboBox = new QFontComboBox(tabFonts);
    fontComboBox->setObjectName(QString::fromUtf8("fontComboBox"));
    fontComboBox->setGeometry(QRect(10, 20, 295, 26));
    lblFont = new QLabel(tabFonts);
    lblFont->setObjectName(QString::fromUtf8("lblFont"));
    lblFont->setGeometry(QRect(10, 0, 142, 18));
    gbColor = new QGroupBox(tabFonts);
    gbColor->setObjectName(QString::fromUtf8("gbColor"));
    gbColor->setGeometry(QRect(0, 80, 371, 101));
    widget2 = new QWidget(gbColor);
    widget2->setObjectName(QString::fromUtf8("widget2"));
    widget2->setGeometry(QRect(10, 25, 351, 66));
    gridLayout1 = new QGridLayout(widget2);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    lblFore = new QLabel(widget2);
    lblFore->setObjectName(QString::fromUtf8("lblFore"));

    gridLayout1->addWidget(lblFore, 0, 0, 1, 1);

    sbFgR = new QSpinBox(widget2);
    sbFgR->setObjectName(QString::fromUtf8("sbFgR"));
    sbFgR->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    sbFgR->setMaximum(255);

    gridLayout1->addWidget(sbFgR, 0, 1, 1, 1);

    sbFgG = new QSpinBox(widget2);
    sbFgG->setObjectName(QString::fromUtf8("sbFgG"));
    sbFgG->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    sbFgG->setMaximum(255);

    gridLayout1->addWidget(sbFgG, 0, 2, 1, 1);

    sbFgB = new QSpinBox(widget2);
    sbFgB->setObjectName(QString::fromUtf8("sbFgB"));
    sbFgB->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    sbFgB->setMaximum(255);

    gridLayout1->addWidget(sbFgB, 0, 3, 1, 1);

    lblBack = new QLabel(widget2);
    lblBack->setObjectName(QString::fromUtf8("lblBack"));

    gridLayout1->addWidget(lblBack, 1, 0, 1, 1);

    sbBgR = new QSpinBox(widget2);
    sbBgR->setObjectName(QString::fromUtf8("sbBgR"));
    sbBgR->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    sbBgR->setMaximum(255);
    sbBgR->setValue(255);

    gridLayout1->addWidget(sbBgR, 1, 1, 1, 1);

    sbBgG = new QSpinBox(widget2);
    sbBgG->setObjectName(QString::fromUtf8("sbBgG"));
    sbBgG->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    sbBgG->setMaximum(255);
    sbBgG->setValue(255);

    gridLayout1->addWidget(sbBgG, 1, 2, 1, 1);

    sbBgB = new QSpinBox(widget2);
    sbBgB->setObjectName(QString::fromUtf8("sbBgB"));
    sbBgB->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    sbBgB->setMaximum(255);
    sbBgB->setValue(255);

    gridLayout1->addWidget(sbBgB, 1, 3, 1, 1);

    txtPreview = new QLineEdit(tabFonts);
    txtPreview->setObjectName(QString::fromUtf8("txtPreview"));
    txtPreview->setGeometry(QRect(6, 185, 360, 40));
    widget3 = new QWidget(tabFonts);
    widget3->setObjectName(QString::fromUtf8("widget3"));
    widget3->setGeometry(QRect(10, 50, 351, 34));
    hboxLayout6 = new QHBoxLayout(widget3);
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    hboxLayout6->setContentsMargins(0, 0, 0, 0);
    lblPtSize = new QLabel(widget3);
    lblPtSize->setObjectName(QString::fromUtf8("lblPtSize"));

    hboxLayout6->addWidget(lblPtSize);

    sbPtSize = new QSpinBox(widget3);
    sbPtSize->setObjectName(QString::fromUtf8("sbPtSize"));
    sbPtSize->setMinimum(6);
    sbPtSize->setMaximum(128);
    sbPtSize->setValue(10);

    hboxLayout6->addWidget(sbPtSize);

    slPtSize = new QSlider(widget3);
    slPtSize->setObjectName(QString::fromUtf8("slPtSize"));
    slPtSize->setMinimum(6);
    slPtSize->setMaximum(128);
    slPtSize->setValue(10);
    slPtSize->setOrientation(Qt::Horizontal);
    slPtSize->setInvertedAppearance(false);
    slPtSize->setInvertedControls(false);
    slPtSize->setTickPosition(QSlider::NoTicks);

    hboxLayout6->addWidget(slPtSize);

    tabs->addTab(tabFonts, QString());
    lblWrapMarkers->setBuddy(cbxWrapMarkers);
    lblUseTabs->setBuddy(cbxUseTabs);
    lblIndentSize->setBuddy(sbIndentSize);

    retranslateUi(QSciTEPrefs);
    QObject::connect(buttonBox, SIGNAL(accepted()), QSciTEPrefs, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), QSciTEPrefs, SLOT(reject()));
    QObject::connect(sbPtSize, SIGNAL(valueChanged(int)), slPtSize, SLOT(setValue(int)));
    QObject::connect(slPtSize, SIGNAL(valueChanged(int)), sbPtSize, SLOT(setValue(int)));

    tabs->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(QSciTEPrefs);
    } // setupUi

    void retranslateUi(QDialog *QSciTEPrefs)
    {
    QSciTEPrefs->setWindowTitle(QApplication::translate("QSciTEPrefs", "Preferences - QSciTE", 0, QApplication::UnicodeUTF8));
    btnDefaults->setText(QApplication::translate("QSciTEPrefs", "Reset all to defaults", 0, QApplication::UnicodeUTF8));
    cbSaveSize->setText(QApplication::translate("QSciTEPrefs", "Save window size/position on exit", 0, QApplication::UnicodeUTF8));
    gbDocMode->setTitle(QApplication::translate("QSciTEPrefs", "Default display settings", 0, QApplication::UnicodeUTF8));
    cbWrapMode->setText(QApplication::translate("QSciTEPrefs", "Wrap long lines", 0, QApplication::UnicodeUTF8));
    lblWrapMarkers->setText(QApplication::translate("QSciTEPrefs", "Wrap indicators:", 0, QApplication::UnicodeUTF8));
    cbxWrapMarkers->clear();
    cbxWrapMarkers->insertItems(0, QStringList()
     << QApplication::translate("QSciTEPrefs", "None", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QSciTEPrefs", "Near text", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QSciTEPrefs", "Near border", 0, QApplication::UnicodeUTF8)
    );
    cbLineNos->setText(QApplication::translate("QSciTEPrefs", "Line numbers", 0, QApplication::UnicodeUTF8));
    lblUseTabs->setText(QApplication::translate("QSciTEPrefs", "Indent using:", 0, QApplication::UnicodeUTF8));
    cbxUseTabs->clear();
    cbxUseTabs->insertItems(0, QStringList()
     << QApplication::translate("QSciTEPrefs", "Spaces", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("QSciTEPrefs", "Tabs", 0, QApplication::UnicodeUTF8)
    );
    lblIndentSize->setText(QApplication::translate("QSciTEPrefs", "Indent size:", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tabGeneral), QApplication::translate("QSciTEPrefs", "General", 0, QApplication::UnicodeUTF8));
    lblTypes->setText(QApplication::translate("QSciTEPrefs", "File types:", 0, QApplication::UnicodeUTF8));
    lblExtns->setText(QApplication::translate("QSciTEPrefs", "Extensions:", 0, QApplication::UnicodeUTF8));
    lblMagic->setText(QApplication::translate("QSciTEPrefs", "First line matches:", 0, QApplication::UnicodeUTF8));
    btnAddExt->setText(QApplication::translate("QSciTEPrefs", "+", 0, QApplication::UnicodeUTF8));
    btnDelExt->setText(QApplication::translate("QSciTEPrefs", "-", 0, QApplication::UnicodeUTF8));
    btnAddMagic->setText(QApplication::translate("QSciTEPrefs", "+", 0, QApplication::UnicodeUTF8));
    btnDelMagic->setText(QApplication::translate("QSciTEPrefs", "-", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tabFileTypes), QApplication::translate("QSciTEPrefs", "File types", 0, QApplication::UnicodeUTF8));
    lblFont->setText(QApplication::translate("QSciTEPrefs", "Font for new windows:", 0, QApplication::UnicodeUTF8));
    gbColor->setTitle(QApplication::translate("QSciTEPrefs", "Plain text color:", 0, QApplication::UnicodeUTF8));
    lblFore->setText(QApplication::translate("QSciTEPrefs", "Foreground:", 0, QApplication::UnicodeUTF8));
    sbFgR->setPrefix(QApplication::translate("QSciTEPrefs", "R: ", 0, QApplication::UnicodeUTF8));
    sbFgG->setPrefix(QApplication::translate("QSciTEPrefs", "G: ", 0, QApplication::UnicodeUTF8));
    sbFgB->setPrefix(QApplication::translate("QSciTEPrefs", "B: ", 0, QApplication::UnicodeUTF8));
    lblBack->setText(QApplication::translate("QSciTEPrefs", "Background:", 0, QApplication::UnicodeUTF8));
    sbBgR->setPrefix(QApplication::translate("QSciTEPrefs", "R: ", 0, QApplication::UnicodeUTF8));
    sbBgG->setPrefix(QApplication::translate("QSciTEPrefs", "G: ", 0, QApplication::UnicodeUTF8));
    sbBgB->setPrefix(QApplication::translate("QSciTEPrefs", "B: ", 0, QApplication::UnicodeUTF8));
    txtPreview->setText(QApplication::translate("QSciTEPrefs", "Sample text", 0, QApplication::UnicodeUTF8));
    lblPtSize->setText(QApplication::translate("QSciTEPrefs", "Size:", 0, QApplication::UnicodeUTF8));
    sbPtSize->setSuffix(QApplication::translate("QSciTEPrefs", " pt", 0, QApplication::UnicodeUTF8));
    tabs->setTabText(tabs->indexOf(tabFonts), QApplication::translate("QSciTEPrefs", "Fonts", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QSciTEPrefs);
    } // retranslateUi

};

namespace Ui {
    class QSciTEPrefs: public Ui_QSciTEPrefs {};
} // namespace Ui

#endif // UI_DLGPREFSUI_H
