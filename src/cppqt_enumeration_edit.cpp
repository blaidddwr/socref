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
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Enumeration::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.enumeration.edit.geometry");

   // Create a new form layout, adding the base name field then the base description 
   // field and then the class field as a checkbox. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addCheckBoxes(ret,{Field::Class},1,"Properties:");

   // Return the form layout. 
   return ret;
}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QString Enumeration::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::Class: return tr("Class");
   default:
      {
         // If the given index is unknown then throw an exception. 
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
