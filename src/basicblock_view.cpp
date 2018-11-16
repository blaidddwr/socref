#include "basicblock_view.h"
#include <QMetaMethod>
#include <socutil/sut_exceptions.h>



using namespace Sut;
//



/*!
 */
const char* BasicBlock::View::_header1Tag {"h1"};
/*!
 */
const char* BasicBlock::View::_header2Tag {"h2"};
/*!
 */
const char* BasicBlock::View::_header3Tag {"h3"};
/*!
 */
const char* BasicBlock::View::_paragraphsTag {"ps"};
/*!
 */
const char* BasicBlock::View::_customTag {"custom"};
/*!
 */
const char* BasicBlock::View::_nameKey {"name"};






/*!
 *
 * @param block The basic block this new view is viewing. 
 */
BasicBlock::View::View(BasicBlock* block):
   _block(block)
{
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
   update();
}






/*!
 */
void BasicBlock::View::update()
{
   QString text;
   enum {Title1,Title2,Title3,Paragraphs,Custom};
   static const QStringList list {_header1Tag,_header2Tag,_header3Tag,_paragraphsTag,_customTag};
   QDomNode node {_block->_viewDefinition.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         switch (list.indexOf(element.tagName()))
         {
         case Title1:
            text += QStringLiteral("<h2>") + element.text() + QStringLiteral("</h2>");
            break;
         case Title2:
            text += QStringLiteral("<h3>") + element.text() + QStringLiteral("</h3>");
            break;
         case Title3:
            text += QStringLiteral("<h4>") + element.text() + QStringLiteral("</h4>");
            break;
         case Paragraphs:
            addParagraphs(&text,element);
            break;
         case Custom:
            addCustom(&text,element);
            break;
         }
      }
      node = node.nextSibling();
   }
   setText(text);
}






/*!
 *
 * @param text  
 *
 * @param element  
 */
void BasicBlock::View::addParagraphs(QString* text, const QDomElement& element)
{
   if ( !element.hasAttribute(_idKey) )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Paragraphs view element missing id attribute."));
      throw e;
   }
   const QVariant field {_block->get(element.attribute(_idKey))};
   if ( field.type() != QVariant::String )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Paragraphs view element referencing baisc block field that is not a string."));
      throw e;
   }
   QStringList paragraphs {field.toString().split(' ',QString::SkipEmptyParts)};
   if ( !paragraphs.isEmpty() )
   {
      *text += QStringLiteral("<p>") + paragraphs.join("</p><p>") + QStringLiteral("</p>");
   }
}






/*!
 *
 * @param text  
 *
 * @param element  
 */
void BasicBlock::View::addCustom(QString* text, const QDomElement& element)
{
   QByteArray signature {element.attribute(_nameKey).toLocal8Bit() + "()"};
   int index {metaObject()->indexOfMethod(QMetaObject::normalizedSignature(signature.data()))};
   if ( index < 0 )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Custom view element referencing unknown method '%1'.").arg(signature.data()));
      throw e;
   }
   QString subtext;
   QMetaMethod method {metaObject()->method(index)};
   method.invoke(this,Qt::DirectConnection,Q_RETURN_ARG(QString,subtext));
   *text += subtext;
}
