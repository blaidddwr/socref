#include "cppqt_signal_edit.h"
#include <QFormLayout>



using namespace Sut;
using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given signal block. 
 *
 * @param block Signal block that this new dialog edits. 
 */
Signal::Edit::Edit(Signal* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Signal::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.signal.edit.geometry");

   // Create a new form layout, adding the base name field and then the base 
   // description field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);

   // Return the form layout. 
   return ret;
}






/*!
 * Implements _Gui::AbstractBlock_ interface. 
 *
 * @param index See interface docs. 
 *
 * @return See interface docs. 
 */
QString Signal::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");

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
