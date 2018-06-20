#include "cppqt_signal_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given variable block. 
 *
 * @param block Variable block that this new edit object edits. 
 */
Signal::Edit::Edit(Signal* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements the interface that is called to return the layout of all GUI elements 
 * used by this dialog object's form layout. 
 *
 * @return Pointer to the layout containing all GUI elements for this dialog. 
 */
QLayout* Signal::Edit::layout()
{
   saveSettings("cppqt.signal.edit.geometry");
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   return ret;
}






/*!
 * Implements the interface that returns the title for the given field type. 
 *
 * @param index The field index whose display title is returned. 
 *
 * @return Title of the given field type. 
 */
QString Signal::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. If the given field index is 
   // undefined then throw an exception. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
