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
 *
 *
 * Steps of Operation: 
 *
 * 1. Save this dialog object's geometry to Qt settings. 
 *
 * 2. Create a new form layout _ret_, add the type edit widget, then add a new line 
 *    edit for the base name field, then add a new text edit for the base 
 *    description field, then add a new line edit for the initial value field, and 
 *    then add a check boxes field for the constant expression, static, and mutable 
 *    fields. 
 *
 * 3. Return _ret_. 
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the given field index return its title. If the given field index is 
 *    undefined then throw an exception. 
 */
QString Signal::Edit::fieldTitle(int index) const
{
   // 1
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
