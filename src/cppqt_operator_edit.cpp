#include "cppqt_operator_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit object with the given operator block. 
 *
 * @param block Operator block that this new edit object edits. 
 */
Operator::Edit::Edit(Operator* block):
   Function::Edit(block)
{}






/*!
 * Implements _Function::Edit::leftLayout_. 
 *
 * @return Pointer to the layout containing all GUI elements for the left side of 
 *         this dialog. 
 */
QLayout* Operator::Edit::leftLayout()
{
   // Save the geometry and state of this function edit dialog. 
   saveSettings("cppqt.operator.edit");

   // Create a new qt form layout that will be returned. 
   QFormLayout* ret {new QFormLayout};

   // Add all edit widgets for this block to the qt form layout. 
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addReturnEdit(ret);
   addTextEdit(ret,Field::ReturnDescription);
   addCheckBoxes(ret
                 ,{Field::ConstExpr
                   ,Field::Static
                   ,Field::Const
                   ,Field::NoExcept
                   ,Field::Default
                   ,Field::Virtual
                   ,Field::Abstract
                   ,Field::Override
                   ,Field::Final}
                 ,5
                 ,"Properties:");

   // Return the qt form layout. 
   return ret;
}
