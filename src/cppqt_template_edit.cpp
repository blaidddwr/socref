#include "cppqt_template_edit.h"
#include <QFormLayout>



using namespace CppQt;
//






/*!
 * Constructs a new edit object with the given template block. 
 *
 * @param block Template block that this new edit object edits. 
 */
Template::Edit::Edit(Template* block):
   Variable::Edit(block)
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
 * 2. Create a new form layout _ret_, add the type edit widget, then add a new line 
 *    edit for the base name field, and then add a new text edit for the base 
 *    description field. 
 *
 * 3. Return _ret_. 
 */
QLayout* Template::Edit::layout()
{
   // 1
   saveSettings("cppqt.template.edit.geometry");

   // 2
   QFormLayout* ret {new QFormLayout};
   addTypeEdit(ret);
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);

   // 3
   return ret;
}
