#include "cppqt_function_edit.h"
#include <QSplitter>
#include <QSettings>
#include <QCloseEvent>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "cppqt_gui_typeselection.h"
#include "gui_listedit.h"
#include "application.h"



using namespace CppQt;
//






/*!
 * Constructs a new function edit object with the given function block. 
 *
 * @param block Function block that this new edit object edits. 
 */
Function::Edit::Edit(Function* block):
   ::Gui::AbstractEdit(block),
   _block(block)
{}






/*!
 * This interface is called to return the layout of all GUI elements used by this 
 * function edit object's form layout positioned to the left of its operations list 
 * edit widget. The default implementation defines all elements for the base 
 * function block. 
 *
 * @return Pointer to the layout containing all GUI elements for the left side of 
 *         this dialog. 
 */
QLayout* Function::Edit::leftLayout()
{
   // Save this dialog object's geometry and state to Qt settings. 
   saveSettings("cppqt.function.edit");

   // Create a new form layout _ret_, add the type edit widget, then add a new line 
   // edit for the base name field, then add a new text edit for the base description 
   // field, then add the return type edit field, then add a new text edit for the 
   // return description field, and then add a check boxes field for all base 
   // function property fields. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addReturnEdit(ret);
   addTextEdit(ret,Field::ReturnDescription);
   addCheckBoxes(ret
                 ,{Field::ConstExpr
                   ,Field::Static
                   ,Field::Const
                   ,Field::NoExcept
                   ,Field::Virtual
                   ,Field::Abstract
                   ,Field::Override
                   ,Field::Final}
                 ,4
                 ,"Properties:");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that is called to return the layout of all GUI elements 
 * used by this dialog object's form layout. This implementation divides the layout 
 * to the left side with a new interface to make it and the right side populated by 
 * the properties list edit widget. 
 *
 * @return Pointer to the layout containing all GUI elements for this dialog. 
 */
QLayout* Function::Edit::layout()
{
   // Create a new qt splitter for this new object, then create the left side widget 
   // _left_, and then add t he left layout to _left_ by calling the left layout 
   // interface. 
   _splitter = new QSplitter(this);
   QWidget* left {new QWidget(this)};
   _splitter->addWidget(left);
   _splitter->addWidget(rightLayout());
   left->setLayout(leftLayout());

   // Add the _left_ widget to this object's splitter and then the right side widget 
   // by calling the right layout method. 

   // Create a new vertical box layout, add this object's qt splitter to it, and then 
   // return the new layout. 
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_splitter);
   return ret;
}






/*!
 * Implements the interface that is called when the user has clicked the apply or 
 * OK buttons and expected to update the block an implementation is editing. 
 */
void Function::Edit::apply()
{
   // Call the abstract edit apply interface and then set this object's parent block 
   // properties to the value of this object's operations list edit widget. If this 
   // object has a return edit widget set for it then set its parent block return 
   // type to the value of the edit widget. 
   ::Gui::AbstractEdit::apply();
   _block->setField(Field::Operations,_operationsEdit->value());
   if ( _returnEdit ) _block->setReturnType(_returnEdit->value());
}






/*!
 * Implements the interface that returns the title for the given field type. 
 *
 * @param index The field index whose display title is returned. 
 *
 * @return Title of the given field type. 
 */
QString Function::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. If the given field index is 
   // undefined then throw an exception. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::Default: return tr("Default");
   case Field::Explicit: return tr("Explicit");
   case Field::Virtual: return tr("Virtual");
   case Field::Const: return tr("Constant");
   case Field::ConstExpr: return tr("Constant Expression");
   case Field::Static: return tr("Static");
   case Field::NoExcept: return tr("No Exceptions");
   case Field::Override: return tr("Override");
   case Field::Final: return tr("Final");
   case Field::Abstract: return tr("Abstract(Pure Virtual)");
   case Field::ReturnType: return tr("Return Type:");
   case Field::ReturnDescription: return tr("Return Description:");
   case Field::Operations: return tr("Operations:");
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}






/*!
 * Implements the qt interface that is called when this dialog is about to close 
 * with the given close event. This implementation uses this event to save the 
 * state of its qt splitter. 
 *
 * @param event Pointer to the qt close event. 
 */
void Function::Edit::closeEvent(QCloseEvent* event)
{
   // Save the state of this edit object's qt splitter with its state key and then 
   // call the abstract edit interface of close event. 
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_stateKey,_splitter->saveState());
   ::Gui::AbstractEdit::closeEvent(event);
}






/*!
 * Sets the base key used by this dialog to save its geometry and state. Additional 
 * strings are appended to this base string to the geometry and state. The given C 
 * string is copied and does not need to be saved after calling this method. 
 *
 * @param baseKey The base key which is copied to generate this object's geometry 
 *                and state keys. 
 */
void Function::Edit::saveSettings(const char* baseKey)
{
   // Set the geometry and state keys for this dialog using the given base key and 
   // appending geometry or state for each key. 
   _geometryKey = QByteArray(baseKey).append(".geometry");
   _stateKey = QByteArray(baseKey).append(".state");

   // Restore the state of this object's qt splitter using the state key and then 
   // call the abstract edit save settings method using the geometry key. 
   QSettings settings(Application::_companyKey,Application::_programKey);
   _splitter->restoreState(settings.value(_stateKey).toByteArray());
   ::Gui::AbstractEdit::saveSettings(_geometryKey.data());
}






/*!
 * Initialize and add the return type edit widget for this function block. 
 *
 * @param form The qt form layout used for this edit dialog that the return type 
 *             edit widget is added to. 
 */
void Function::Edit::addReturnEdit(QFormLayout* form)
{
   // Create and initialize this object's return type edit widget and then add it to 
   // the given qt form layout. 
   _returnEdit = new Gui::TypeSelection(_block,this);
   _returnEdit->setValue(_block->returnType());
   form->addRow(new QLabel(tr("Return Type:")),_returnEdit);
}






/*!
 * Construct and initializes the right side of this function edit dialog. This 
 * simply creates the operations list edit widget which takes up the whole right 
 * side. 
 *
 * @return Pointer to the widget that contains the right side layout for this new 
 *         edit dialog. 
 */
QWidget* Function::Edit::rightLayout()
{
   // Create and initialize the operations list edit widget for this new dialog. 
   _operationsEdit = new ::Gui::ListEdit(this);
   _operationsEdit->setValue(_block->operations());

   // Create a new vertical layout _layout_, adding a properties label and then the 
   // operations list edit widget. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(new QLabel(tr("Properties"),this));
   layout->addWidget(_operationsEdit);

   // Create a new widget, set its layout to _layout_, and then return its pointer. 
   QWidget* ret {new QWidget(this)};
   ret->setLayout(layout);
   return ret;
}
