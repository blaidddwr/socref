#include "cppqt_enumvalue_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given enumeration value block. 
 *
 * @param block Enumeration value block that this new edit object edits. 
 */
EnumValue::Edit::Edit(EnumValue* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* EnumValue::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.enumvalue.edit.geometry");

   // Create a new form layout, adding the base name field then the base description 
   // field and then the value field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addLineEdit(ret,Field::Value);

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
QString EnumValue::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::Value: return tr("Value:");

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
