#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "cppqt_gui_settingsdialog.h"
#include "cppqt_settings.h"



using namespace CppQt::Gui;
//






SettingsDialog::SettingsDialog():
   ::Gui::PersistentDialog("cppqt.gui.settingsdialog.geometry")
{
   setupGui();
   setWindowTitle(tr("C++/Qt Settings"));
}






void SettingsDialog::okClicked()
{
   applyClicked();
   accept();
}






void SettingsDialog::applyClicked()
{
   Settings& settings {Settings::instance()};
   settings.setIndentSpaces(_indentSpacesBox->value());
   settings.setHeaderLines(_headerLinesBox->value());
   settings.setFunctionLines(_functionLinesBox->value());
   settings.setMaxColumns(_maxColumnsBox->value());
}






void SettingsDialog::setupGui()
{
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupForm());
   layout->addLayout(setupButtons());
   setLayout(layout);
}






QLayout* SettingsDialog::setupForm()
{
   QFormLayout* ret {new QFormLayout};
   addIndentSpaces(ret);
   addHeaderLines(ret);
   addFunctionLines(ret);
   addMaxColumns(ret);
   return ret;
}






void SettingsDialog::addIndentSpaces(QFormLayout* layout)
{
   _indentSpacesBox = new QSpinBox;
   _indentSpacesBox->setValue(Settings::instance().indentSpaces());
   layout->addRow(new QLabel(tr("Indent Spacing:")),_indentSpacesBox);
}






void SettingsDialog::addHeaderLines(QFormLayout* layout)
{
   _headerLinesBox = new QSpinBox;
   _headerLinesBox->setValue(Settings::instance().headerLines());
   layout->addRow(new QLabel(tr("Header Lines:")),_headerLinesBox);
}






void SettingsDialog::addFunctionLines(QFormLayout* layout)
{
   _functionLinesBox = new QSpinBox;
   _functionLinesBox->setValue(Settings::instance().functionLines());
   layout->addRow(new QLabel(tr("Function Header Lines:")),_functionLinesBox);
}






void SettingsDialog::addMaxColumns(QFormLayout* layout)
{
   _maxColumnsBox = new QSpinBox;
   _maxColumnsBox->setMaximum(999);
   _maxColumnsBox->setValue(Settings::instance().maxColumns());
   layout->addRow(new QLabel(tr("Maximum Columns:")),_maxColumnsBox);
}






QLayout* SettingsDialog::setupButtons()
{
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(setupOkButton());
   ret->addWidget(setupApplyButton());
   ret->addStretch();
   ret->addWidget(setupCancelButton());
   return ret;
}






QWidget* SettingsDialog::setupOkButton()
{
   QPushButton* ret {new QPushButton(tr("&Ok"))};
   connect(ret,&QPushButton::clicked,this,&SettingsDialog::okClicked);
   return ret;
}






QWidget* SettingsDialog::setupApplyButton()
{
   QPushButton* ret {new QPushButton(tr("&Apply"))};
   connect(ret,&QPushButton::clicked,this,&SettingsDialog::applyClicked);
   return ret;
}






QWidget* SettingsDialog::setupCancelButton()
{
   QPushButton* ret {new QPushButton(tr("&Cancel"))};
   connect(ret,&QPushButton::clicked,this,&SettingsDialog::reject);
   return ret;
}
