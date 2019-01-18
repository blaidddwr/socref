#include "glsl_settings_dialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "glsl_settings.h"



using namespace GLSL;
//






/*!
 * Constructs a new settings dialog. 
 */
Settings::Dialog::Dialog():
   Gui::PersistentDialog("cppqt.gui.settingsdialog.geometry")
{
   // Create the GUI for this new dialog and set its window title. 
   setupGui();
   setWindowTitle(tr("GLSL Settings"));
}






/*!
 * Called when the OK button is clicked. This applies all global settings and 
 * closes the dialog. 
 */
void Settings::Dialog::okClicked()
{
   // Apply all global settings of this dialog and close it. 
   applyClicked();
   done(QDialog::Accepted);
}






/*!
 * Called when the apply button is clicked. This applies the global setting values 
 * of this dialog. 
 */
void Settings::Dialog::applyClicked()
{
   // Get the global settings object and apply all settings. 
   Settings& settings {instance()};
   settings.setVariableLines(_variableLinesBox->value());
   settings.setStructLines(_structLinesBox->value());
   settings.setFunctionLines(_functionLinesBox->value());
   settings.setIndentSpaces(_indentSpacesBox->value());
   settings.setMaxColumns(_maxColumnsBox->value());
}






/*!
 * Creates and initializes all GUI elements for this new dialog. 
 */
void Settings::Dialog::setupGui()
{
   // Create a new vertical layout, adding the form layout and then the buttons 
   // layout. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addLayout(setupForm());
   layout->addLayout(setupButtons());

   // Set the layout of this new dialog. 
   setLayout(layout);
}






/*!
 * Creates and initializes all edit widgets for this new dialog, returning a form 
 * layout containing the edit widgets. 
 *
 * @return Form layout containing all edit widgets for this new dialog. 
 */
QLayout* Settings::Dialog::setupForm()
{
   // Get the global settings instance. 
   Settings& settings {instance()};

   // Create the variable lines edit widget and set its value to the current global 
   // setting. 
   _variableLinesBox = new QSpinBox;
   _variableLinesBox->setValue(settings.variableLines());

   // Create the structure lines edit widget and set its value to the current global 
   // setting. 
   _structLinesBox = new QSpinBox;
   _structLinesBox->setValue(settings.structLines());

   // Create the function lines edit widget and set its value to the current global 
   // setting. 
   _functionLinesBox = new QSpinBox;
   _functionLinesBox->setValue(settings.functionLines());

   // Create the indent spaces edit widget and set its value to the current global 
   // setting. 
   _indentSpacesBox = new QSpinBox;
   _indentSpacesBox->setValue(settings.indentSpaces());

   // Create the max columns edit widget and set its value to the current global 
   // setting. 
   _maxColumnsBox = new QSpinBox;
   _maxColumnsBox->setValue(settings.maxColumns());

   // Create a new form layout, adding the indent spaces widget then the header lines 
   // widget then the function lines widget and then the max columns widget. 
   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("Variable Lines:")),_variableLinesBox);
   ret->addRow(new QLabel(tr("Structure Lines:")),_structLinesBox);
   ret->addRow(new QLabel(tr("Function Lines:")),_functionLinesBox);
   ret->addRow(new QLabel(tr("Indent Spacing:")),_indentSpacesBox);
   ret->addRow(new QLabel(tr("Maximum Columns:")),_maxColumnsBox);

   // Return the form layout. 
   return ret;
}






/*!
 * Creates and initializes the OK, apply, and cancel buttons for this new dialog, 
 * returning the buttons layout. 
 *
 * @return Layout containing buttons for this new dialog. 
 */
QLayout* Settings::Dialog::setupButtons()
{
   // Create the OK button for this dialog, connecting its clicked signal. 
   QPushButton* ok {new QPushButton(tr("&Ok"))};
   connect(ok,&QPushButton::clicked,this,&Dialog::okClicked);

   // Create the apply button for this dialog, connecting its clicked signal. 
   QPushButton* apply {new QPushButton(tr("&Apply"))};
   connect(apply,&QPushButton::clicked,this,&Dialog::applyClicked);

   // Create the cancel button for this dialog, connecting its clicked signal. 
   QPushButton* cancel {new QPushButton(tr("&Cancel"))};
   connect(cancel,&QPushButton::clicked,[this]{ done(QDialog::Rejected); });

   // Create a new horizontal layout, adding the OK button then the apply button then 
   // a stretch and then the cancel button. 
   QHBoxLayout* ret {new QHBoxLayout};
   ret->addWidget(ok);
   ret->addWidget(apply);
   ret->addStretch();
   ret->addWidget(cancel);

   // Return the buttons layout. 
   return ret;
}
