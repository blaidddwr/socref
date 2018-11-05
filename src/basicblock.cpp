#include "basicblock.h"
#include <QIcon>
#include <socutil/sut_exceptions.h>



using namespace Sut;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int BasicBlock::type() const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   return _type;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& BasicBlock::factory() const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   return *_factory;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString BasicBlock::name() const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   if ( _nameFieldId.isEmpty() ) return QString();
   const QVariant field {_fields.value(_nameFieldId)};
   if ( field.type() != QVariant::String )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Name field id '%1' points to invalid field.").arg(_nameFieldId));
      throw e;
   }
   return field.toString();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon BasicBlock::icon() const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   return QIcon(_iconPath);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> BasicBlock::buildList() const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   return _buildList;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<QWidget> BasicBlock::makeView() const
{}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<::Gui::AbstractEdit> BasicBlock::makeEdit()
{}






/*!
 *
 * @param type  
 *
 * @param factory  
 *
 * @param element  
 *
 * @param buildList  
 *
 * @param isDefault  
 */
void BasicBlock::initialize(int type, const AbstractBlockFactory* factory, const QDomElement& element, const QList<int>& buildList, bool isDefault)
{
   if ( _type != -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to initialize basic block object that is already initialized."));
      throw e;
   }
   _type = type;
   _factory = factory;
   _buildList = buildList;
   _definition = element;
   enum {Bool,String,StringList,Name,Icon,Edit,View};
   static const QStringList tags
   {
      "bool","string","stringlist","name","icon","edit","view"
   };
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement child {node.toElement()};
         switch (tags.indexOf(child.tagName()))
         {
         case Bool:
            addField(Field::Bool,child,isDefault);
            break;
         case String:
            addField(Field::String,child,isDefault);
            break;
         case StringList:
            addField(Field::StringList,child,isDefault);
            break;
         case Name:
            _nameFieldId = child.text();
            break;
         case Icon:
            _iconPath = child.text();
            break;
         case Edit:
            _editDefinition = child;
            break;
         case View:
            _viewDefinition = child;
            break;
         }
      }
      node = node.nextSibling();
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param element The XML element used to read in this blocks data. 
 */
void BasicBlock::readData(const QDomElement& element)
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement child {node.toElement()};
         auto i {_fields.find(child.tagName())};
         if ( i != _fields.end() )
         {
            switch ( i->type() )
            {
            case QVariant::Bool:
               *i = QVariant(true);
               break;
            case QVariant::String:
               *i = QVariant(child.text());
               break;
            case QVariant::StringList:
               *i = i->toStringList() << child.text();
               break;
            default:
               Exception::LogicError e;
               SUT_MARK_EXCEPTION(e);
               e.setDetails(tr("Encountered basic block field of unsupported type."));
               throw e;
            }
         }
      }
      node = node.nextSibling();
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param document XML document to use for creating new elements. 
 *
 * @return See interface docs. 
 */
QDomElement BasicBlock::writeData(QDomDocument& document) const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   QDomElement ret {document.createElement("na")};
   for (auto i = _fields.cbegin(); i != _fields.cend() ;++i)
   {
      switch (i->type())
      {
      case QVariant::Bool:
         if ( i->toBool() ) ret.appendChild(document.createElement(i.key()));
         break;
      case QVariant::String:
         {
            QDomElement string {document.createElement(i.key())};
            string.appendChild(document.createTextNode(i->toString()));
            ret.appendChild(string);
            break;
         }
      case QVariant::StringList:
         {
            const QStringList list {i->toStringList()};
            for (auto string: list)
            {
               QDomElement stringlist {document.createElement(i.key())};
               stringlist.appendChild(document.createTextNode(string));
               ret.appendChild(stringlist);
            }
            break;
         }
      default:
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Encountered basic block field of unsupported type."));
         throw e;
      }
   }
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlock::makeBlank() const
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   QPtr<BasicBlock> ret {makeBlankBasic()};
   if ( !ret ) ret.reset(new BasicBlock);
   ret->initialize(_type,_factory,_definition,_buildList,false);
   return ret;
}






/*!
 */
Sut::QPtr<BasicBlock> BasicBlock::makeBlankBasic() const
{
   return nullptr;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param other The other block whose data will be copied. 
 */
void BasicBlock::copyDataFrom(const AbstractBlock* other)
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   const BasicBlock* basic {qobject_cast<const BasicBlock*>(other)};
   if ( !basic )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given abstract block to copy is not a basic block."));
      throw e;
   }
   for (auto i = basic->_fields.cbegin(); i != basic->_fields.cend() ;++i)
   {
      auto j {_fields.find(i.key())};
      if ( j == _fields.end() )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Missing field to copy from given basic block."));
         throw e;
      }
      if ( i->type() != j->type() )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Field type mismatch with given basic block."));
         throw e;
      }
      *j = *i;
   }
}






/*!
 *
 * @param id  
 */
bool BasicBlock::getBool(const QString& id)
{
   const QVariant field {get(id)};
   if ( field.type() != QVariant::Bool )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' is not a boolean."));
      throw e;
   }
   return field.toBool();
}






/*!
 *
 * @param id  
 */
QString BasicBlock::getString(const QString& id)
{
   const QVariant field {get(id)};
   if ( field.type() != QVariant::String )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' is not a string."));
      throw e;
   }
   return field.toString();
}






/*!
 *
 * @param id  
 */
QStringList BasicBlock::getStringList(const QString& id)
{
   const QVariant field {get(id)};
   if ( field.type() != QVariant::StringList )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' is not a string list."));
      throw e;
   }
   return field.toStringList();
}






/*!
 *
 * @param id  
 */
QVariant BasicBlock::get(const QString& id)
{
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
   auto i {_fields.find(id)};
   if ( i == _fields.end() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' does not exist."));
      throw e;
   }
   return *i;
}






/*!
 *
 * @param type  
 *
 * @param element  
 *
 * @param isDefault  
 */
void BasicBlock::addField(Field type, const QDomElement& element, bool isDefault)
{
   const QString key {element.attribute("id")};
   if ( !key.isEmpty() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Field definition element %1 missing id attribute.").arg(element.tagName()));
      throw e;
   }
   if ( _fields.contains(key) )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Field definition element %1 redefining id '%2'.")
                   .arg(element.tagName())
                   .arg(key));
      throw e;
   }
   switch (type)
   {
   case Field::Bool:
      if ( isDefault )
      {
         _fields.insert(key,QVariant( element.attribute("default").toLower() == QStringLiteral("true") ? true : false));
      }
      else _fields.insert(key,QVariant(false));
      break;
   case Field::String:
      if ( isDefault ) _fields.insert(key,QVariant(element.attribute("default")));
      else _fields.insert(key,QVariant(QString()));
      break;
   case Field::StringList:
      _fields.insert(key,QVariant(QStringList()));
      break;
   }
}
