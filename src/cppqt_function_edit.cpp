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
   // Save the geometry and state of this dialog. 
   saveSettings("cppqt.function.edit");

   // Create a new form layout, adding the base name field then the base description 
   // field then the return type field and then the return description field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addReturnEdit(ret);
   addTextEdit(ret,Field::ReturnDescription);

   // Add a check boxes widget for all function property fields. 
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

   // Return the form layout. 
   return ret;
}






/*!
 * Implements _Gui::AbstractBlock_ interface. This implementation divides the 
 * layout to the left side with a new interface to make it and the right side 
 * populated by the properties list edit widget. 
 *
 * @return See interface docs. 
 */
QLayout* Function::Edit::layout()
{
   // Create the splitter for this new edit dialog. 
   _splitter = new QSplitter(this);

   // Create a widget for the left side of this dialog because the splitter widget 
   // can only have widgets added to it. 
   QWidget* left {new QWidget(this)};
   _splitter->addWidget(left);

   // Add the right layout to the splitter and then set the left side widget's 
   // layout. CAUTION: The right side must be created first since the left side might 
   // use objects created when the right side is created. 
   _splitter->addWidget(rightLayout());
   left->setLayout(leftLayout());

   // Create and return a new vertical box layout, adding this object's splitter to 
   // it. 
   QVBoxLayout* ret {new QVBoxLayout};
   ret->addWidget(_splitter);
   return ret;
}






/*!
 * Implements _Gui::AbstractEdit_ interface. This implementation sets the 
 * operations field and possibly the optional custom return type field and then 
 * passes back to its base interface. 
 */
void Function::Edit::apply()
{
   // Apply the operations field to this edit's function block. 
   _block->setField(Field::Operations,_operationsEdit->value());

   // If the return type edit widget exists then apply the return type field to this 
   // edit's function block. 
   if ( _returnEdit ) _block->setReturnType(_returnEdit->value());

   // Call this object's base interface. 
   ::Gui::AbstractEdit::apply();
}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QString Function::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
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

   // If the given field index is unknown then throw an exception. 
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
 * Implements _QWidget_ interface. This implementation uses this event to save the 
 * state of its splitter widget and then calls its parent interface. 
 *
 * @param event See Qt docs. 
 */
void Function::Edit::closeEvent(QCloseEvent* event)
{
   // Save the state of this edit object's splitter with its state key. 
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_stateKey,_splitter->saveState());

   // Call this object's parent interface. 
   ::Gui::AbstractEdit::closeEvent(event);
}






/*!
 * Sets the base key used by this dialog to save its geometry and state and 
 * restores its geometry and state. Additional strings are appended to this base 
 * string to the geometry and state. The given C string is copied and does not need 
 * to be saved after calling this method. 
 *
 * @param baseKey The base key which is copied to generate this object's geometry 
 *                and state keys. 
 */
void Function::Edit::saveSettings(const char* baseKey)
{
   // Set the geometry and state keys for this dialog using the given base key. 
   _geometryKey = QByteArray(baseKey).append(".geometry");
   _stateKey = QByteArray(baseKey).append(".state");

   // Restore the state of this object's splitter using the state key. 
   QSettings settings(Application::_companyKey,Application::_programKey);
   _splitter->restoreState(settings.value(_stateKey).toByteArray());

   // Call the abstract edit save settings method using the geometry key. 
   ::Gui::AbstractEdit::saveSettings(_geometryKey.data());
}






/*!
 * Initialize and add the return type edit widget for this function block to the 
 * given form layout. 
 *
 * @param form The qt form layout used for this edit dialog that the return type 
 *             edit widget is added to. 
 */
void Function::Edit::addReturnEdit(QFormLayout* form)
{
   // Create and initialize this object's return type edit widget. 
   _returnEdit = new Gui::TypeSelection(_block,this);
   _returnEdit->setValue(_block->returnType());

   // Add the return type edit widget to the given form layout. 
   form->addRow(new QLabel(tr("Return Type:")),_returnEdit);
}






/*!
 * Constructs and initializes the right side of this function edit dialog. This 
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

   // Create a new vertical layout, adding a properties label and then the operations 
   // list edit widget. 
   QVBoxLayout* layout {new QVBoxLayout};
   layout->addWidget(new QLabel(tr("Properties"),this));
   layout->addWidget(_operationsEdit);

   // Create and return a new widget, setting its layout to the operations layout. 
   QWidget* ret {new QWidget(this)};
   ret->setLayout(layout);
   return ret;
}
