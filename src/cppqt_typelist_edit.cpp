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
 * Implements the interface that is called to return the layout of all GUI elements 
 * used by this dialog object's form layout. 
 *
 * @return Pointer to the layout containing all GUI elements for this dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Save this dialog object's geometry to Qt settings. 
 *
 * 2. Create a new form layout, add a new line edit for the base name field, then 
 *    add a new text edit for the base description field, and then return the new 
 *    form layout. 
 */
QLayout* TypeList::Edit::layout()
{
   // 1
   saveSettings("cppqt.typelist.edit.geometry");

   // 2
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Field::Name);
   addTextEdit(ret,Field::Description);
   return ret;
}






/*!
 * Implements the interface that returns the title for the given field type. 
 *
 * @param index  
 *
 * @return Title of the given field type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given field index return its title. If the given field index is 
 *    undefined then throw an exception. 
 */
QString TypeList::Edit::fieldTitle(int index) const
{
   // 1
   switch (index)
   {
   case Field::Name: return tr("Title:");
   case Field::Description: return tr("Type Info:");
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Invalid field index %1.").arg(index));
         throw e;
      }
   }
}
