#include "cppqt_base_view.h"
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"



using namespace CppQt;
//






/*!
 * Constructs a new base view with the given base block. 
 *
 * @param block Constant pointer to the base block this new view is showing. 
 */
Base::View::View(const Base* block):
   _block(block)
{
   // Initialize this label object's alignment, word wrapping, text format, and 
   // margin. 
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
}






/*!
 * Parses the bold marker character '_' from the given text, replacing it with HTML 
 * bold tags. The parsed text is returned. 
 *
 * @param text Text that is parsed to replace '_' with HTML bold tags. 
 *
 * @return Parsed text replacing '_' with HTML bold tags. 
 */
QString Base::View::parseBoldMarkers(const QString& text)
{
   // Create an empty string _ret_ and initialize the ingress state to true. 
   QString ret;
   bool ingress {true};

   // Iterate through all characters of the given text. If the next character is not 
   // a bold marker then append it to _ret_. Else if the character is a bold marker 
   // then append an HTML bold tag to _ret_, using and flipping the ingress state to 
   // determine if it should be a beginning or ending bold tag. 
   for (auto ch: text)
   {
      if ( ch == QChar('_') )
      {
         if ( ingress ) ret.append("<b>");
         else ret.append("</b>");
         ingress = !ingress;
      }
      else ret.append(ch);
   }

   // Return _ret_. 
   return ret;
}






/*!
 * Returns an HTML string containing the description of this view's base block. The 
 * HTML returned contains a title and one or more paragraphs of the base block's 
 * description field. If this view has no base block this returns an empty string. 
 *
 * @return HTML string containing the description of this view's base block or an 
 *         empty string if this object has no base block. 
 */
QString Base::View::displayDescription()
{
   // Create an empty string _ret_. If this object's block pointer is null then 
   // return _ret_. 
   QString ret;
   if ( !_block ) return ret;

   // Split this view's base block's description field into paragraphs using double 
   // newlines as the separator, saving the list of paragraphs to _paragraphs_. If 
   // the list of paragraphs is empty then return _ret_. 
   QStringList paragraphs {_block->description().split("\n\n",QString::SkipEmptyParts)};
   if ( paragraphs.isEmpty() ) return ret;

   // Add a HTML title to _ret_ and then add all strings in the string list 
   // _paragraphs_ as HTML paragraphs to _ret_. 
   ret.append("<h3>Description</h3><p>");
   for (int i = 0; i < (paragraphs.size() - 1) ;++i)
   {
      ret.append(parseBoldMarkers(paragraphs.at(i))).append("</p><p>");
   }
   ret.append(paragraphs.last()).append("</p>");

   // Return _ret_. 
   return ret;
}






/*!
 * Returns a HTML string containing a list of any template arguments this view's 
 * block may contain. Each template argument also has its own description added to 
 * the HTML string. If this view has no base block this returns an empty string. 
 *
 * @return HTML string containing list of any template arguments this view's block 
 *         may contain or an empty string if this object has no base block. 
 */
QString Base::View::displayTemplates()
{
   // Create an empty string _ret_. If this object's block pointer is null then 
   // return _ret_. 
   QString ret;
   if ( !_block ) return ret;

   // Create a pointer list _list_ of all template blocks this view's block contains 
   // as direct children. If _list_ is empty then return _ret_. 
   const QList<Template*> list {_block->makeListOfType<Template>(BlockFactory::TemplateType)};
   if ( list.isEmpty() ) return ret;

   // Add a HTML title to _ret_ and then add all template child blocks as a HTML 
   // paragraph using their variable type name, base name, and description. 
   ret.append("<h3>Templates</h3>");
   for (auto template_ : list)
   {
      ret.append("<p>")
         .append(template_->variableType())
         .append(" <b>")
         .append(template_->Base::name())
         .append("</b> : ")
         .append(template_->description())
         .append("</p>");
   }

   // Return _ret_. 
   return ret;
}
