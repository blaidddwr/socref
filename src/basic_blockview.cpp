#include "basic_blockview.h"
#include <QDomElement>
#include <QMetaMethod>
#include "basic_block.h"



namespace Basic
{



/*!
 * The tag name for a header 1 display element, 1 being the largest.
 */
const char* BlockView::_header1Tag {"h1"};
/*!
 * The tag name for a header 2 display element, 1 being the largest.
 */
const char* BlockView::_header2Tag {"h2"};
/*!
 * The tag name for a header 3 display element, 1 being the largest.
 */
const char* BlockView::_header3Tag {"h3"};
/*!
 * The tag name for a paragraphs display element that displays a basic block
 * field as multiple paragraphs separated by double newline characters.
 */
const char* BlockView::_paragraphsTag {"ps"};
/*!
 * The tag name for a custom display element specifying a custom method to call
 * that returned a custom string.
 */
const char* BlockView::_customTag {"custom"};
/*!
 * The attribute name for the custom method name of a custom display element.
 */
const char* BlockView::_nameKey {"name"};






/*!
 * Constructs a new view with the given basic block which is displays.
 *
 * @param block The basic block this new view is displaying.
 */
BlockView::BlockView(const Block* block)
   :
   Abstract::BlockView(block)
{
   // Format this new view's label.
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
}






/*!
 * Implements _Abstract::BlockView_ interface.
 */
void BlockView::renderText()
{
   // Create a new string that will hold the new rich text of this view.
   QString text;

   // Prepare an enumeration and matching string list for recognizing element tag
   // names.
   enum {Title1,Title2,Title3,Paragraphs,Custom};
   static const QStringList list {_header1Tag,_header2Tag,_header3Tag,_paragraphsTag,_customTag};

   // Iterate through all child nodes of this view's basic block's view definition
   // element.
   QDomNode node {block<Block>()._viewDefinition.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if the node is an element.
      if ( node.isElement() )
      {
         // Figure out which display element this is, if any, and add its string to the new
         // updated rich text.
         QDomElement element {node.toElement()};
         switch (list.indexOf(element.tagName()))
         {
         case Title1:
            text += QStringLiteral("<h1>") + element.text() + QStringLiteral("</h1>");
            break;
         case Title2:
            text += QStringLiteral("<h2>") + element.text() + QStringLiteral("</h2>");
            break;
         case Title3:
            text += QStringLiteral("<h3>") + element.text() + QStringLiteral("</h3>");
            break;
         case Paragraphs:
            addParagraphs(&text,element);
            break;
         case Custom:
            addCustom(&text,element);
            break;
         }
      }

      // Move to the next node sibling.
      node = node.nextSibling();
   }

   // Set the text of this view's label to the updated rich text.
   setText(text);
}






/*!
 * Parses all underline characters from the given list of strings, replacing
 * them with HTML bold tags. The parsed list of strings is returned.
 *
 * @param list List of strings that is parsed to replace underline characters
 *             with HTML bold tags.
 *
 * @return Parsed list of strings, replacing underlines with HTML bold tags.
 */
QStringList BlockView::parseBoldMarkers(const QStringList& list)
{
   // Create an empty string list that will be returned.
   QStringList ret;

   // Iterate through all strings of the given string list.
   for (auto text: list)
   {
      // Create an empty parsed string and initialize the ingress state.
      QString parsed;
      bool ingress {true};

      // Iterate through every character of the text.
      for (auto ch: text)
      {
         // Check if the current character is an underline.
         if ( ch == QChar('_') )
         {
            // Append an opening or closing HTML old tag to the parsed string depending on the
            // ingress state.
            if ( ingress ) parsed.append("<b>");
            else parsed.append("</b>");

            // Invert the ingress state.
            ingress = !ingress;
         }

         // Else the current character is not an underline so simply append it to the
         // parsed string.
         else parsed.append(ch);
      }

      // Add the parsed string to the list of parsed strings returned.
      ret << parsed;
   }

   // Return the parsed string list.
   return ret;
}






/*!
 * Adds paragraphs to the given rich text based off the given XML display
 * element definition. If the basic block field referenced by the given display
 * element then nothing will be added. The basic block field referenced must be
 * a string or an exception is thrown.
 *
 * @param text Pointer to the rich text that paragraphs are added to if the
 *             given display element's basic block field is not empty.
 *
 * @param element XML display element definition that references the basic block
 *                field to add as paragraphs to the given rich text.
 */
void BlockView::addParagraphs(QString* text, const QDomElement& element)
{
   // Make sure the given display element has the id attribute.
   Q_ASSERT(element.hasAttribute(Block::_idKey));

   // Get the basic block field that the given display element references and make
   // sure it is a string.
   const QVariant field {block<Block>().get(element.attribute(Block::_idKey))};
   Q_ASSERT(field.type() == QVariant::String);

   // Split the basic block field into a string list of paragraphs based off double
   // newline characters being the separator, parsing each string for bold underline
   // markers.
   QStringList paragraphs
   {
      parseBoldMarkers(field.toString().split("\n\n",QString::SkipEmptyParts))
   };

   // Make sure the list of paragraphs is not empty.
   if ( !paragraphs.isEmpty() )
   {
      // Add the string list to the given rich text as paragraphs.
      *text += QStringLiteral("<p>") + paragraphs.join("</p><p>") + QStringLiteral("</p>");
   }
}






/*!
 * Adds custom content to the given rich text based off the given XML custom
 * display element. If the provided method name does not exist for this object
 * then an exception is thrown.
 *
 * @param text The rich text that custom content is added to.
 *
 * @param element XML custom display element definition that provides the custom
 *                method name to call and is expected to return a string of
 *                custom content.
 */
void BlockView::addCustom(QString* text, const QDomElement& element)
{
   // Lookup the custom method referenced by the name attribute of the given custom
   // display element definition, making sure the method exists.
   QByteArray signature {element.attribute(_nameKey).toLocal8Bit() + "()"};
   int index {metaObject()->indexOfMethod(QMetaObject::normalizedSignature(signature.data()))};
   Q_ASSERT(index >= 0);

   // Invoke the custom method of this object and add the content it returns to the
   // given rich text.
   QString subtext;
   QMetaMethod method {metaObject()->method(index)};
   method.invoke(this,Qt::DirectConnection,Q_RETURN_ARG(QString,subtext));
   *text += subtext;
}

}
