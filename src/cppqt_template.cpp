#include "cppqt_template.h"
#include "cppqt_template_view.h"
#include "cppqt_template_edit.h"
#include "cppqt_blockfactory.h"



using namespace Sut;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Template::type() const
{
   return BlockFactory::TemplateType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Template::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/template.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<QWidget> Template::makeView() const
{
   return QPtr<QWidget>(new View(this));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<::Gui::AbstractEdit> Template::makeEdit()
{
   return QPtr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new template block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Template::Template(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault )
   {
      setType("class");
      setName("T");
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> Template::makeBlank() const
{
   return QPtr<AbstractBlock>(new Template);
}
