#include "cppqt_class_view.h"
#include "cppqt_parent.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given variable block. 
 *
 * @param block Variable block this new view displays. 
 */
Class::View::View(const Class* block):
   Base::View(block),
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's variable block. 
 *
 * @return HTML rich text that displays the body of this view's variable block. 
 */
QString Class::View::displayText()
{
   return displayDescription().append(displayQtObject())
                              .append(displayTemplates())
                              .append(displayParents());
}






/*!
 */
QString Class::View::displayQtObject()
{
   //
   return tr("<p><b>Qt Object</b> : %1</p>").arg(_block->isQtObject() ? "Yes" : "No");
}






/*!
 * Returns a HTML string that displays any properties this view's variable block 
 * has set. If this view's variable block has no properties set then this returns 
 * an empty string. 
 *
 * @return HTML string that displays any properties this view's variable block has 
 *         set or an empty string if no properties are set. 
 */
QString Class::View::displayParents()
{
   //
   QString ret;
   const QList<Parent*> list {_block->parents()};

   //
   if ( list.isEmpty() ) return ret;

   //
   ret.append("<h3>Inheritance</h3>");

   //
   for (auto parent : list)
   {
      //
      ret.append(tr("<p><b>%1</b> %2</p>")
                 .arg(parent->accessString())
                 .arg(parent->className().replace("<","&lt;")));
   }

   //
   return ret;
}
