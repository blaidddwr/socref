#include "cppqt_slot_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit dialog with the given slot block. 
 *
 * @param block Slot block that this new dialog edits. 
 */
Slot::Edit::Edit(Slot* block):
   Function::Edit(block)
{}






/*!
 * Implements _Function::Edit::leftLayout_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Slot::Edit::leftLayout()
{
   // Save the geometry and state of this dialog. 
   saveSettings("cppqt.slot.edit");

   // Create a new form layout, adding the base name field and then the base 
   // description field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);

   // Append all function properties relevant to the slot type as a check boxes 
   // widget. 
   addCheckBoxes(ret
                 ,{Field::ConstExpr
                   ,Field::Static
                   ,Field::Const
                   ,Field::NoExcept
                   ,Field::Virtual
                   ,Field::Abstract
                   ,Field::Override
                   ,Field::Final}
                 ,4
                 ,"Properties:");

   // Return the form layout. 
   return ret;
}
