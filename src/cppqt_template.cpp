#include "cppqt_template.h"
#include "cppqt_template_view.h"
#include "cppqt_template_edit.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Template::type() const
{
   return BlockFactory::TemplateType;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the static qt icon _ret_ is null then load the icon for this block type. 
 *
 * 2. Return _ret_. 
 */
QIcon Template::icon() const
{
   // 1
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/template.svg");

   // 2
   return ret;
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Template::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Template::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new template block with the given name. 
 *
 * @param name The value for this new template block's base name field. 
 */
Template::Template(const QString& name):
   Variable(name)
{}






/*!
 * Constructs a new template block with the given type and name. 
 *
 * @param type The value of this template block's variable type field. 
 *
 * @param name The value for this new template block's base name field. 
 */
Template::Template(const QString& type, const QString& name):
   Variable(type,name)
{}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Template::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Template);
}
