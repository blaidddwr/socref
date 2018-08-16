#include "cppqt_using_edit.h"
#include <QFormLayout>
#include "cppqt_blockfactory.h"



using namespace Sut;
using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given using block. 
 *
 * @param block Using block that this new dialog edits. 
 */
Using::Edit::Edit(Using* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Using::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.using.edit.geometry");

   // Create a new form layout, adding the field field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Field::FieldType);

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
QString Using::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Field::FieldType: return tr("Field:");

   // If the given index is unknown then throw an exception. 
   default:
      {
         Exception::InvalidArgument e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
