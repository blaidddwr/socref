#include "cppqt_settings_dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "cppqt_settings.h"



using namespace CppQt;
//






/*!
 * Constructs a new settings dialog. 
 */
Settings::Dialog::Dialog():
   ::Gui::PersistentDialog("cppqt.gui.settingsdialog.geometry")
{
   // Create the GUI for this new dialog and set its window title. 
   setupGui();
   setWindowTitle(tr("C++/Qt Settings"));
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
   settings.setIndentSpaces(_indentSpacesBox->value());
   settings.setHeaderLines(_headerLinesBox->value());
   settings.setFunctionLines(_functionLinesBox->value());
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

   // Create the indent spaces edit widget and set its value to the current global 
   // setting. 
   _indentSpacesBox = new QSpinBox;
   _indentSpacesBox->setValue(settings.indentSpaces());

   // Create the header lines edit widget and set its value to the current global 
   // setting. 
   _headerLinesBox = new QSpinBox;
   _headerLinesBox->setValue(settings.headerLines());

   // Create the function lines edit widget and set its value to the current global 
   // setting. 
   _functionLinesBox = new QSpinBox;
   _functionLinesBox->setValue(settings.functionLines());

   // Create the max columns edit widget and set its value to the current global 
   // setting. 
   _maxColumnsBox = new QSpinBox;
   _maxColumnsBox->setValue(settings.maxColumns());

   // Create a new form layout, adding the indent spaces widget then the header lines 
   // widget then the function lines widget and then the max columns widget. 
   QFormLayout* ret {new QFormLayout};
   ret->addRow(new QLabel(tr("Indent Spacing:")),_indentSpacesBox);
   ret->addRow(new QLabel(tr("Header Lines:")),_headerLinesBox);
   ret->addRow(new QLabel(tr("Function Header Lines:")),_functionLinesBox);
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
   connect(apply,&QPushButton::clicked,this,&Dialog::okClicked);

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
