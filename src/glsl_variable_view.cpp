#include "glsl_variable_view.h"



using namespace GLSL;
//






/*!
 * Constructs a new view with the given variable block. 
 *
 * @param block The variable block that this new view displays. 
 */
Variable::View::View(const Variable* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays a list of all layout qualifiers for this view's 
 * variable block. If this view's variable block has no layout qualifiers then this 
 * returns an empty string. 
 *
 * @return Rich text that displays a list of all layout qualifiers for this view's 
 *         variable block or an empty string if there are no qualifiers. 
 */
QString Variable::View::displayLayout()
{
   // Create a new empty string. 
   QString ret;

   // Get the list of layout qualifiers for this view's variable block and make sure 
   // it is not empty. 
   const QStringList layout {_block->layout()};
   if ( !layout.isEmpty() )
   {
      // Add a title to the returned rich text. 
      ret += QStringLiteral("<h3>Layout Qualifiers</h3><p>");

      // Add all individual qualifiers to the rich text on a new line for each one. 
      for (auto qualifier: layout)
      {
         ret += qualifier + QStringLiteral("<br/>");
      }

      // Close the rich text paragraph. 
      ret += QStringLiteral("</p>");
   }

   // Return the layout rich text or empty string if there is no layouts. 
   return ret;
}
