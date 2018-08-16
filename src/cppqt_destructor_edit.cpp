#include "cppqt_destructor_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given destructor block. 
 *
 * @param block Destructor block that this new dialog edits. 
 */
Destructor::Edit::Edit(Destructor* block):
   Function::Edit(block)
{}






/*!
 * Implements _Function::Edit::leftLayout_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Destructor::Edit::leftLayout()
{
   // Save the geometry and state of this dialog. 
   saveSettings("cppqt.destructor.edit");

   // Create a new form layout, adding the base description field and then the 
   // function properties used by this block type. 
   QFormLayout* ret {new QFormLayout};
   addTextEdit(ret,Base::Field::Description);
   addCheckBoxes(ret
                 ,{Field::Virtual
                   ,Field::Abstract
                   ,Field::Override
                   ,Field::Final
                   ,Field::NoExcept
                   ,Field::Default}
                 ,4
                 ,"Properties:");

   // Return the form layout. 
   return ret;
}
