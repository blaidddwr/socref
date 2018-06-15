#include "cppqt_destructor_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit object with the given operator block. 
 *
 * @param block Operator block that this new edit object edits. 
 */
Destructor::Edit::Edit(Destructor* block):
   Function::Edit(block)
{}






/*!
 * Implements _Function::Edit::leftLayout_. 
 *
 * @return Pointer to the layout containing all GUI elements for the left side of 
 *         this dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Save the geometry and state of this function edit dialog. 
 *
 * 2. Create a new qt form layout that will be returned. 
 *
 * 3. Add all edit widgets for this block to the qt form layout. 
 *
 * 4. Return the qt form layout. 
 */
QLayout* Destructor::Edit::leftLayout()
{
   // 1
   saveSettings("cppqt.destructor.edit");

   // 2
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

   // 3
   return ret;
}
