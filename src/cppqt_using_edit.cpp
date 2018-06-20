#include "cppqt_using_edit.h"
#include <QFormLayout>
#include "cppqt_blockfactory.h"



using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given variable block. 
 *
 * @param block Variable block that this new edit object edits. 
 */
Using::Edit::Edit(Using* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements the interface that is called to return the layout of all GUI elements 
 * used by this dialog object's form layout. 
 *
 * @return Pointer to the layout containing all GUI elements for this dialog. 
 */
QLayout* Using::Edit::layout()
{
   saveSettings("cppqt.using.edit.geometry");
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Field::FieldType);
   return ret;
}






/*!
 * Implements the interface that returns the title for the given field type. 
 *
 * @param index The field index whose display title is returned. 
 *
 * @return Title of the given field type. 
 */
QString Using::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. If the given field index is 
   // undefined then throw an exception. 
   switch (index)
   {
   case Field::FieldType: return tr("Field:");
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
