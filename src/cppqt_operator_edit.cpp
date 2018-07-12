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
 * Implements _Function::Edit::leftLayout_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Operator::Edit::leftLayout()
{
   // Save the geometry and state of this dialog. 
   saveSettings("cppqt.operator.edit");

   // Create a new form layout, adding the base name field then the base description 
   // field then the return type custom widget and then the return type description 
   // field. 
   QFormLayout* ret {new QFormLayout};
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);
   addReturnEdit(ret);
   addTextEdit(ret,Field::ReturnDescription);

   // Add a check boxes widget for all operator property fields. 
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

   // Return the form layout. 
   return ret;
}
