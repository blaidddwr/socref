#include "cppqt_enumeration_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given enumeration block. 
 *
 * @param block Enumeration block that this new edit object edits. 
 */
Enumeration::Edit::Edit(Enumeration* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements the interface that is called to return the layout of all GUI elements 
 * used by this dialog object's form layout. 
 *
 * @return Pointer to the layout containing all GUI elements for this dialog. 
 */
QLayout* Enumeration::Edit::layout()
{
   // Save this dialog object's geometry to Qt settings. 
   saveSettings("cppqt.enumeration.edit.geometry");

   // Create a new form layout _ret_, add a new line edit for the base name field, 
   // and then add a new text edit for the base description field, and then add a 
   // check boxes layout for the class field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addCheckBoxes(ret,{Field::Class},1,"Properties:");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that returns the title for the given field type. 
 *
 * @param index The field index whose display title is returned. 
 *
 * @return Title of the given field type. 
 */
QString Enumeration::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. If the given field index is 
   // undefined then throw an exception. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::Class: return tr("Class");
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
