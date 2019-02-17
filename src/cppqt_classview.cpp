#include "cppqt_classview.h"
#include "cppqt_class.h"
#include "cppqt_parent.h"
#include "cppqt_template.h"



namespace CppQt
{






/*!
 * Constructs a new view object with the given class block.
 *
 * @param block Class block this new view displays.
 */
ClassView::ClassView(const Class* block)
   :
   Basic::BlockView(block)
{
}






/*!
 * Returns rich text that displays the sole qt object property of this view's
 * class block.
 *
 * @return Rich text that displays the qt object property of this view's class
 *         block.
 */
QString ClassView::displayQtObject()
{
   // Return qt object property state of this view's class block as rich text.
   return tr("<p><b>Qt Object</b> : %1</p>").arg(block<Class>().isQtObject() ? "Yes" : "No");
}






/*!
 * Returns rich text that displays all inherited parent classes of this view's
 * class block. If this view's class block does not inherit any classes then
 * this returns an empty string.
 *
 * @return Rich text that displays the inheritance of this view's class block or
 *         an empty string if it has no inheritance.
 */
QString ClassView::displayParents()
{
   // Create an empty return string.
   QString ret;

   // Make a pointer list of child parent blocks this view's class block contains and
   // make sure it is not empty.
   const QList<Parent*> list {block<Class>().parents()};
   if ( list.isEmpty() ) return ret;

   // Append a rich text title.
   ret += QStringLiteral("<h3>Inheritance</h3>");

   // Iterate through all of the parent block pointer list and append each one as
   // rich text.
   for (auto parent : list)
   {
      ret += QStringLiteral("<p><b>")
           + parent->accessString()
           + QStringLiteral("</b> ")
           + parent->className().replace("<","&lt;")
           + QStringLiteral("</p>");
   }

   // Return the inheritance rich text.
   return ret;
}






/*!
 * Wrapper method that calls the static method in _CppQt::Template_ of the same
 * name.
 *
 * @return See wrapped method docs.
 */
QString ClassView::displayTemplates()
{
   return Template::displayTemplates(block<Class>());
}

}
