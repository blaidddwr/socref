#include "cppqt_parent_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given parent block. 
 *
 * @param block Parent block that this new dialog edits. 
 */
Parent::Edit::Edit(Parent* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Parent::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.parent.edit.geometry");

   // Create a new form layout, adding the access type field and then the class name 
   // field. 
   QFormLayout* ret {new QFormLayout};
   addComboEdit(ret,Field::AccessType,_accessNames);
   addLineEdit(ret,Field::ClassName);

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
QString Parent::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Field::AccessType: return tr("Access:");
   case Field::ClassName: return tr("Class:");

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
