#include "cppqt_access_edit.h"
#include <QFormLayout>



using namespace Sut;
using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given access block. 
 *
 * @param block Access block that this new dialog edits. 
 */
Access::Edit::Edit(Access* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Access::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.access.edit.geometry");

   // Create a new form layout, adding the access type field as a combo box. 
   QFormLayout* ret {new QFormLayout};
   addComboEdit(ret,Field::AccessType,_typeNames);

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
QString Access::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Field::AccessType: return tr("Type:");

   // If the given field index is unknown then throw an exception. 
   default:
      {
         Exception::InvalidArgument e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
