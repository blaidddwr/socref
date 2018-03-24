#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "cppqt_gui_settingsdialog.h"
#include "application.h"



using namespace CppQt::Gui;
const char* SettingsDialog::_indentSpacesKey {"settings.cppqt.indent.spaces"};
const char* SettingsDialog::_headerLinesKey {"settings.cppqt.header.lines"};
const char* SettingsDialog::_functionLinesKey {"settings.cppqt.header.functionlines"};
//






SettingsDialog::SettingsDialog():
   ::Gui::PersistentDialog("cppqt.gui.settingsdialog.geometry"),
   _settings(Application::_companyKey,Application::_programKey)
{
   setupGui();
   setWindowTitle(tr("C++/Qt Settings"));
}






int SettingsDialog::indentSpaces()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   return settings.value(_indentSpacesKey).toInt();
}






int SettingsDialog::headerLines()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   return settings.value(_headerLinesKey).toInt();
}






int SettingsDialog::functionLines()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   return settings.value(_functionLinesKey).toInt();
}






void SettingsDialog::okClicked()
{
   applyClicked();
   accept();
}






void SettingsDialog::applyClicked()
{
   _settings.setValue(_indentSpacesKey,_indentSpacesBox->value());
   _settings.setValue(_headerLinesKey,_headerLinesBox->value());
   _settings.setValue(_functionLinesKey,_functionLinesBox->value());
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
   return ret;
}






void SettingsDialog::addIndentSpaces(QFormLayout* layout)
{
   _indentSpacesBox = new QSpinBox;
   _indentSpacesBox->setValue(_settings.value(_indentSpacesKey).toInt());
   layout->addRow(new QLabel(tr("Indent Spacing:")),_indentSpacesBox);
}






void SettingsDialog::addHeaderLines(QFormLayout* layout)
{
   _headerLinesBox = new QSpinBox;
   _headerLinesBox->setValue(_settings.value(_headerLinesKey).toInt());
   layout->addRow(new QLabel(tr("Header Lines:")),_headerLinesBox);
}






void SettingsDialog::addFunctionLines(QFormLayout* layout)
{
   _functionLinesBox = new QSpinBox;
   _functionLinesBox->setValue(_settings.value(_functionLinesKey).toInt());
   layout->addRow(new QLabel(tr("Function Header Lines:")),_functionLinesBox);
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
