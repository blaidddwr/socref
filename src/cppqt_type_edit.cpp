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
   Gui::AbstractEdit(block)
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
 * 2. Create a new form layout, add a new line edit for the type name field, and 
 *    then return the new form layout. While adding the type name field connect the 
 *    qt line edit's text changed signal to this object's appropriate slot. 
 */
QLayout* Type::Edit::layout()
{
   // 1
   saveSettings("cppqt.type.edit.geometry");

   // 2
   QFormLayout* ret {new QFormLayout};
   connect(addLineEdit(ret,Field::TypeName),&QLineEdit::textChanged,this,&Edit::textChanged);
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
QString Type::Edit::fieldTitle(int index) const
{
   // 1
   switch (index)
   {
   case Field::TypeName: return "Type:";
   default:
      {
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
