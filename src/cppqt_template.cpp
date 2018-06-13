#include "cppqt_template.h"
#include "cppqt_view_template.h"
#include "cppqt_edit_template.h"
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
 */
QIcon Template::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/template.svg");
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
   return unique_ptr<QWidget>(new View::Template(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Template::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Template(this));
}






/*!
 */
Template::Template()
{}






/*!
 *
 * @param name  
 */
Template::Template(const QString& name):
   Variable(name)
{}






/*!
 *
 * @param type  
 *
 * @param name  
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
