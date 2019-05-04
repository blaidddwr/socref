#include "glsl_variableview.h"
#include "glsl_variable.h"



namespace GLSL
{






/*!
 * Constructs a new view with the given variable block.
 *
 * @param block The variable block that this new view displays.
 */
VariableView::VariableView(const Variable* block)
   :
   Basic::BlockView(block)
{
}






/*!
 * Returns rich text that displays a list of all layout qualifiers for this
 * view's variable block. If this view's variable block has no layout qualifiers
 * then this returns an empty string.
 *
 * @return Rich text that displays a list of all layout qualifiers for this
 *         view's variable block or an empty string if there are no qualifiers.
 */
QString VariableView::displayLayout()
{
   // Create a new empty string.
   QString ret;

   // Get the list of layout qualifiers for this view's variable block and make sure
   // it is not empty.
   const QStringList layout {block<Variable>().layout()};
   if ( !layout.isEmpty() )
   {
      // Add a title to the returned rich text.
      ret += QStringLiteral("<h3>") + tr("Layout Qualifiers") + QStringLiteral("</h3><p>");

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

}
