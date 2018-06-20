#include "cppqt_constructor_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit object with the given operator block. 
 *
 * @param block Operator block that this new edit object edits. 
 */
Constructor::Edit::Edit(Constructor* block):
   Function::Edit(block)
{}






/*!
 * Implements _Function::Edit::leftLayout_. 
 *
 * @return Pointer to the layout containing all GUI elements for the left side of 
 *         this dialog. 
 */
QLayout* Constructor::Edit::leftLayout()
{
   // Save the geometry and state of this function edit dialog. 
   saveSettings("cppqt.constructor.edit");

   // Create a new qt form layout that will be returned. 
   QFormLayout* ret {new QFormLayout};
   addTextEdit(ret,Base::Field::Description);
   addCheckBoxes(ret
                 ,{Field::NoExcept
                   ,Field::Explicit
                   ,Field::Default}
                 ,3
                 ,"Properties:");

   // Add all edit widgets for this block to the qt form layout. 
   return ret;
}
