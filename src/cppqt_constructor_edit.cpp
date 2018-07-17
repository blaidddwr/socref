#include "cppqt_constructor_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given constructor block. 
 *
 * @param block Constructor block that this new dialog edits. 
 */
Constructor::Edit::Edit(Constructor* block):
   Function::Edit(block)
{}






/*!
 * Implements _Function::Edit::leftLayout_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Constructor::Edit::leftLayout()
{
   // Save the geometry and state of this dialog. 
   saveSettings("cppqt.constructor.edit");

   // Create a new form layout, adding the base description field and then the 
   // function properties used by this block type. 
   QFormLayout* ret {new QFormLayout};
   addTextEdit(ret,Base::Field::Description);
   addCheckBoxes(ret
                 ,{Field::NoExcept
                   ,Field::Explicit
                   ,Field::Default}
                 ,3
                 ,"Properties:");

   // Return the form layout. 
   return ret;
}
