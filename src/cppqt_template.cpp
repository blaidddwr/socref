#include "cppqt_template.h"
#include "cppqt_factory.h"



using namespace CppQt;
//






/*!
 * Returns rich text containing a list of any template arguments the given block 
 * may contain. Each template argument also has its own description added to the 
 * rich text. 
 *
 * @param block The base block whose direct block children are searched for 
 *              template blocks whose information is added to returned rich text. 
 *
 * @return Rich text containing list of any template arguments the given block may 
 *         contain. 
 */
QString Template::displayTemplates(const Base& block)
{
   // Create an empty string that will contain the rich text. 
   QString ret;

   // Create a pointer list of all template blocks this view's block contains as 
   // direct children, making sure that list is not empty. 
   const QList<Template*> list {block.makeListOfType<Template>(Factory::TemplateType)};
   if ( list.isEmpty() ) return ret;

   // Append the rich text title for templates. 
   ret += QStringLiteral("<h3>Templates</h3>");

   // Iterate through all template block pointers and append a rich text paragraph 
   // describing each one. 
   for (auto templateBlock : list)
   {
      ret += QStringLiteral("<p>")
           + templateBlock->templateType()
           + QStringLiteral(" <b>")
           + templateBlock->baseName()
           + QStringLiteral("</b> : ")
           + templateBlock->description()
           + QStringLiteral("</p>");
   }

   // Return the rich text. 
   return ret;
}






/*!
 * Returns the template type of this template block. 
 *
 * @return Template type of this template block. 
 */
QString Template::templateType() const
{
   return getString("type");
}
