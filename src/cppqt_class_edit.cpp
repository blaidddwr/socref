#include "cppqt_class_edit.h"
#include <QFormLayout>



using namespace Sut;
using namespace CppQt;
//






/*!
 * Constructs a new type edit object with the given class block. 
 *
 * @param block Class block that this new edit object edits. 
 */
Class::Edit::Edit(Class* block):
   ::Gui::AbstractEdit(block)
{}






/*!
 * Implements _Gui::AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Class::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.class.edit.geometry");

   // Create a new form layout, adding the base name field then the base description 
   // field and then a check box for the qt object field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addCheckBoxes(ret,{Field::QtObject},1,"Properties:");

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
QString Class::Edit::fieldTitle(int index) const
{
   // Based off the given field index return its title. 
   switch (index)
   {
   case Base::Field::Name: return tr("Name:");
   case Base::Field::Description: return tr("Description:");
   case Field::QtObject: return tr("Qt Object");

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
