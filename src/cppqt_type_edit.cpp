#include "cppqt_type_edit.h"
#include <QFormLayout>
#include <QLineEdit>



using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given type block. 
 *
 * @param block Type block that this new edit object edits. 
 */
Type::Edit::Edit(Type* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Type::Edit::layout()
{
   // Save the geometry for this dialog. 
   saveSettings("cppqt.type.edit.geometry");

   // Create a new form layout, adding the type name field and connecting its text 
   // changed signal. 
   QFormLayout* ret {new QFormLayout};
   connect(addLineEdit(ret,Field::TypeName),&QLineEdit::textChanged,this,&Edit::textChanged);

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
QString Type::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Field::TypeName: return tr("Type:");
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






/*!
 * Called when the text in this edit object's line edit widget for the type name 
 * field has changed. This enables or disables the OK/apply buttons of its dialog 
 * depending on if the text is a valid type name or not. 
 *
 * @param text The new text that the qt line edit widget contains. 
 */
void Type::Edit::textChanged(const QString& text)
{
   setDisabled(!CppQt::Type::isValidTypeString(text));
}
