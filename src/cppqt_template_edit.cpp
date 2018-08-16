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
 * Implements _Gui::AbstractEdit_ interface. 
 *
 * @return See interface docs. 
 */
QLayout* Template::Edit::layout()
{
   // Save the geometry of this dialog. 
   saveSettings("cppqt.template.edit.geometry");

   // Create a new form layout, adding the variable type edit widget then the base 
   // name field and then the base description field. 
   QFormLayout* ret {new QFormLayout};
   addTypeEdit(ret);
   addLineEdit(ret,Base::Field::Name);
   addTextEdit(ret,Base::Field::Description);

   // Return the form layout. 
   return ret;
}
