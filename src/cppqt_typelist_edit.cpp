#include "cppqt_typelist_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new type list edit object with the given type list block. 
 *
 * @param block Type list block that this new edit object edits. 
 */
TypeList::Edit::Edit(TypeList* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* TypeList::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.typelist.edit.geometry");

   // Create a new form layout, adding the base name field and then the base 
   // description field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Field::Name);
   addTextEdit(ret,Field::Description);

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
QString TypeList::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Field::Name: return tr("Title:");
   case Field::Description: return tr("Type Info:");
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
