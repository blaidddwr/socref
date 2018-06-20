#include "cppqt_variable_edit.h"
#include <QFormLayout>
#include <QLabel>
#include "cppqt_gui_typeselection.h"



using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given variable block. 
 *
 * @param block Variable block that this new edit object edits. 
 */
Variable::Edit::Edit(Variable* block):
   ::Gui::AbstractEdit(block),
   _block(block)
{}






/*!
 * Implements the interface that is called to return the layout of all GUI elements 
 * used by this dialog object's form layout. 
 *
 * @return Pointer to the layout containing all GUI elements for this dialog. 
 */
QLayout* Variable::Edit::layout()
{
   // Save this dialog object's geometry to Qt settings. 
   saveSettings("cppqt.variable.edit.geometry");

   // Create a new form layout _ret_, add the type edit widget, then add a new line 
   // edit for the base name field, then add a new text edit for the base description 
   // field, then add a new line edit for the initial value field, and then add a 
   // check boxes field for the constant expression, static, and mutable fields. 
   QFormLayout* ret {new QFormLayout};

   // Return _ret_. 
   addTypeEdit(ret);
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addLineEdit(ret,Field::Initializer);
   addCheckBoxes(ret,{Field::ConstExpr,Field::Static,Field::Mutable},4,"Properties:");

   // 
   return ret;
}






/*!
 * Implements the interface that is called when the user has clicked the apply or 
 * OK buttons and expected to update the block an implementation is editing. 
 */
void Variable::Edit::apply()
{
   // Call the abstract edit interface. If this object's type edit widget is valid 
   // then set this object's variable block's type field to the value from the type 
   // edit widget. 
   if ( _typeEdit ) _block->setField(Field::Type,_typeEdit->value());

   // 
   ::Gui::AbstractEdit::apply();
}






/*!
 * Implements the interface that returns the title for the given field type. 
 *
 * @param index The field index whose display title is returned. 
 *
 * @return Title of the given field type. 
 */
QString Variable::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. If the given field index is 
   // undefined then throw an exception. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::ConstExpr: return tr("Constant Expression");
   case Field::Static: return tr("Static");
   case Field::Mutable: return tr("Mutable");
   case Field::Initializer: return tr("Initializer:");
   default:

      // 
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
   // Create and initialize this object's type edit widget and then add it to the 
   // given qt form layout. 
   _typeEdit = new Gui::TypeSelection(_block);
   _typeEdit->setValue(_block->variableType());
   form->addRow(new QLabel(tr("Type:")),_typeEdit);
}
