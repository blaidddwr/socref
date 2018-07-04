#include "cppqt_base_view.h"
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"



using namespace CppQt;
//






/*!
 * Constructs a new base view object with no associated base block. 
 */
Base::View::View()
{
   setupLabel();
}






/*!
 * Constructs a new base view with the given base block. 
 *
 * @param block Constant pointer to the base block this new view is showing. 
 */
Base::View::View(const Base* block):
   _block(block)
{
   setupLabel();
}






/*!
 * Parses all underline characters from the given text, replacing it with HTML bold 
 * tags. The parsed text is returned. 
 *
 * @param text Text that is parsed to replace underline characters with HTML bold 
 *             tags. 
 *
 * @return Parsed text replacing underlines with HTML bold tags. 
 */
QString Base::View::parseBoldMarkers(const QString& text)
{
   // Create an empty string and initialize the ingress state. 
   QString ret;
   bool ingress {true};

   // Iterate through every character of the given text. 
   for (auto ch: text)
   {
      // Check if the current character is an underline. 
      if ( ch == QChar('_') )
      {
         // If ingress is true then append a beginning HTML bold marker else append a 
         // ending HTML bold marker. 
         if ( ingress ) ret.append("<b>");
         else ret.append("</b>");

         // Invert the ingress state. 
         ingress = !ingress;
      }

      // Else the current character is not an underline so simply append it. 
      else ret.append(ch);
   }

   // Return the bold HTML formatted string. 
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
   // Create an empty string and make sure this view has a block pointer. 
   QString ret;
   if ( !_block ) return ret;

   // Split this view's block's description field into paragraphs using double 
   // newlines as the separator and make sure the there is at least one paragraph. 
   QStringList paragraphs {_block->description().split("\n\n",QString::SkipEmptyParts)};
   if ( paragraphs.isEmpty() ) return ret;

   // Append the HTML title for the description. 
   ret.append("<h3>Description</h3><p>");

   // Iterate through almost every paragraph except for the very last one. 
   for (int i = 0; i < (paragraphs.size() - 1) ;++i)
   {
      // Append the paragraph with underline bold markers parsed and separating HTML 
      // paragraph markers. 
      ret.append(parseBoldMarkers(paragraphs.at(i))).append("</p><p>");
   }

   // Append the very last paragraph with underline markers parsed and the final 
   // ending paragraph HTML tag. 
   ret.append(parseBoldMarkers(paragraphs.last())).append("</p>");

   // Return the HTML description string. 
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
   // Create an empty string and make sure this view has a block pointer. 
   QString ret;
   if ( !_block ) return ret;

   // Create a pointer list of all template blocks this view's block contains as 
   // direct children, making sure that list is not empty. 
   const QList<Template*> list {_block->makeListOfType<Template>(BlockFactory::TemplateType)};
   if ( list.isEmpty() ) return ret;

   // Append the HTML title for templates. 
   ret.append("<h3>Templates</h3>");

   // Iterate through all template block pointers and append an HTML paragraph 
   // describing each one. 
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

   // Return the HTML templates string. 
   return ret;
}






/*!
 * Initializes the properties of this base label widget. 
 */
void Base::View::setupLabel()
{
   // Initialize this label widget. 
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
}
