#include "cppqt_variable_edit.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_gui_typeselection.h"



using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given variable block. 
 *
 * @param block Variable block that this new edit object edits. 
 */
Variable::Edit::Edit(Variable* block):
   ::Gui::AbstractEdit(block),
   _block(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Variable::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.variable.edit.geometry");

   // Create a new form layout, adding the variable type field then the base name 
   // field and then the base description field. 
   QFormLayout* ret {new QFormLayout};
   addTypeEdit(ret);
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addLineEdit(ret,Field::Initializer);

   // Add the constant expression, static, and mutable properties as check box 
   // properties. 
   addCheckBoxes(ret,{Field::ConstExpr,Field::Static,Field::Mutable},4,"Properties:");

   // Return the form layout. 
   return ret;
}






/*!
 * Implements _Gui::AbstractEdit_ interface. This implementation handles the custom 
 * type edit widget and then passes onto to the base interface. 
 */
void Variable::Edit::apply()
{
   // If this dialog has a custom type edit widget then apply its value to the block 
   // this dialog is editing. 
   if ( _typeEdit ) _block->setField(Field::Type,_typeEdit->value());

   // Call the base interface for this dialog to handle all other edit widgets. 
   ::Gui::AbstractEdit::apply();
}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QString Variable::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::ConstExpr: return tr("Constant Expression");
   case Field::Static: return tr("Static");
   case Field::Mutable: return tr("Mutable");
   case Field::Initializer: return tr("Initializer:");

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
 * Initialize and add the type edit widget for this edit block. 
 *
 * @param form The qt form layout used for this edit dialog that the type edit 
 *             widget is added to. 
 */
void Variable::Edit::addTypeEdit(QFormLayout* form)
{
   // Create and initialize this object's type edit widget, adding it to the given qt 
   // form layout. 
   _typeEdit = new Gui::TypeSelection(_block);
   _typeEdit->setValue(_block->variableType());
   form->addRow(new QLabel(tr("Type:")),_typeEdit);
}
