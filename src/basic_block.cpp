#include "basic_block.h"
#include <QIcon>
#include <QJsonObject>
#include <socutil/ReadError>
#include "basic_blockedit.h"
#include "basic_blockview.h"
using Soc::Ut::ReadError;



namespace Basic
{



/*!
 * The boolean tag name of elements that define a boolean data field.
 */
const QString Block::_boolTag {"bool"};
/*!
 * The string tag name of elements that define a string data field.
 */
const QString Block::_stringTag {"string"};
/*!
 * The string list tag name of elements that define a string list data field.
 */
const QString Block::_stringListTag {"stringlist"};
/*!
 * The tag name of the element used to define the name option that specifies
 * which string field is used for this block's name.
 */
const QString Block::_nameTag {"name"};
/*!
 * The tag name of the element used to define the path for this basic block
 * type's icon.
 */
const QString Block::_iconTag {"icon"};
/*!
 * The tag name of the element that hold the configuration for this basic block
 * type's edit class.
 */
const QString Block::_editTag {"edit"};
/*!
 * The tag name of the element that hold the configuration for this basic block
 * type's view class.
 */
const QString Block::_viewTag {"view"};
/*!
 * The attribute name for the id of data field element definitions.
 */
const QString Block::_idKey {"id"};
/*!
 * The attribute name for default values of data field element definitions.
 */
const QString Block::_defaultKey {"default"};






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
int Block::type() const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Return the block type of this basic block.
   return _type;
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
const Abstract::BlockFactory& Block::factory() const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Return a reference to this basic block's factory.
   return *_factory;
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QString Block::name() const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Make sure this basic block's name field id is not empty.
   if ( _nameFieldId.isEmpty() ) return QString();

   // Get this basic block's field with its name field id, making it exists and is
   // the correct field type.
   const QVariant field {_fields.value(_nameFieldId)};
   Q_ASSERT(field.type() == QVariant::String);

   // Return the found string field.
   return field.toString();
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QIcon Block::icon() const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Return this basic block type's icon.
   return QIcon(_iconPath);
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QList<int> Block::buildList() const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Return this basic block type's build list.
   return _buildList;
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Abstract::BlockView> Block::createView() const
{
   Soc::Ut::QPtr<Basic::BlockView> ret(makeBasicView());
   ret->update();
   return std::move(ret);
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Abstract::BlockEdit> Block::createEdit()
{
   return new BlockEdit(this);
}






/*!
 * Initializes this basic block to the given type. This must be called before
 * this basic block can be used and cannot be called again.
 *
 * @param type The block type of this new basic block.
 *
 * @param factory The basic block factory that made this new basic block.
 *
 * @param element The XML element that defines all data fields and options of
 *                the basic block type this new basic block is being initialized
 *                to.
 *
 * @param buildList The build list of the basic block type this new basic block
 *                  is being initialized to.
 *
 * @param isDefault True if the data fields should be set to default values or
 *                  false to set them to their null values.
 */
void Block::initialize(int type, const Abstract::BlockFactory* factory, const QDomElement& element, const QList<int>& buildList, bool isDefault)
{
   // Make sure this basic block has NOT been initialized.
   Q_ASSERT(_type == -1);

   // Set the type, factory, and build list for this new basic block.
   _type = type;
   _factory = factory;
   _buildList = buildList;

   // Save the XML element definition for making copies of this basic block.
   _definition = element;

   // Prepare an enumeration and string list that stores all possible element tag
   // names that are recognized.
   enum {Bool,String,StringList,Name,Icon,Edit,View};
   static const QStringList tags
   {
      _boolTag,_stringTag,_stringListTag,_nameTag,_iconTag,_editTag,_viewTag
   };

   // Iterate through all node children of the given XML definition element.
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if the node is an element.
      if ( node.isElement() )
      {
         // Determine if the tag name of this element is recognized as a configuration
         // option, adding the field or setting the option if it is.
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

      // Move to the next sibling node.
      node = node.nextSibling();
   }
}






/*!
 * This interface returns a new basic block view implementation for this basic
 * block. The default interface returns a generic basic block view. This should
 * be used for basic block views that use the custom view element in its
 * definition.
 *
 * @return New basic block view implementation for this basic block.
 */
Soc::Ut::QPtr<Basic::BlockView> Block::makeBasicView() const
{
   return new BlockView(this);
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @param element See interface docs.
 */
void Block::readData(const QDomElement& element)
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Iterate through all node children of the given data element.
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      // Check to see if the node is an element.
      if ( node.isElement() )
      {
         // Find the data field with the id that matches the element's tag name, making
         // sure it exists.
         QDomElement child {node.toElement()};
         auto i {_fields.find(child.tagName())};
         if ( i != _fields.end() )
         {
            // Set the data field based off what type it is, throwing an exception if it is an
            // unknown type.
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
               throw ReadError(qUtf8Printable(tr("Unknown data element '%1' on line %2 within block element on line %3.")
                                              .arg(child.tagName())
                                              .arg(child.lineNumber())
                                              .arg(element.lineNumber()))
                               ,"");
            }
         }
      }

      // Move to the next sibling node.
      node = node.nextSibling();
   }
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @param document See interface docs.
 *
 * @return See interface docs.
 */
QDomElement Block::writeData(QDomDocument& document) const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Create the return data element.
   QDomElement ret {document.createElement("na")};

   // Iterate through all data fields of this basic block.
   for (auto i = _fields.cbegin(); i != _fields.cend() ;++i)
   {
      // Based off the data field type write out its data as child elements appended to
      // the data element to be returned, marking sure the field type is recognized.
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
         Q_ASSERT(false);
      }
   }

   // Return the complete data element that stores this basic block's field data.
   return ret;
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Abstract::Block> Block::createBlank() const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Create a new uninitialized basic block, making sure it worked.
   Soc::Ut::QPtr<Block> ret {qobject_cast<Block*>(metaObject()->newInstance())};
   Q_ASSERT(ret.get());

   // Initialize the new basic block to this basic block's type.
   ret->initialize(_type,_factory,_definition,_buildList,false);

   // Return the initialized basic block.
   return std::move(ret);
}






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @param other See interface docs.
 */
void Block::copyDataFrom(const Abstract::Block* other)
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Cast the given abstract block to a basic block, making sure it worked.
   const Block* basic {qobject_cast<const Block*>(other)};
   Q_ASSERT(basic);

   // Iterate through all the fields of the given basic block.
   for (auto i = basic->_fields.cbegin(); i != basic->_fields.cend() ;++i)
   {
      // Find the corresponding field of this basic block, making sure it was found.
      auto j {_fields.find(i.key())};
      Q_ASSERT(j != _fields.end());

      // Make sure the field types matched between this and the other basic block.
      Q_ASSERT(i->type() == j->type());

      // Copy the value of the given basic block field to this basic block.
      *j = *i;
   }
}






/*!
 * Returns the value of this basic block's field with the given id. The given id
 * must exist and it must be a boolean.
 *
 * @param id The id of this basic block's field whose value is returned.
 *
 * @return Value of this basic block's field with the given id.
 */
bool Block::getBool(const QString& id) const
{
   // Get the data field of this basic block with the given id, making sure it is the
   // correct type.
   const QVariant field {get(id)};
   Q_ASSERT(field.type() == QVariant::Bool);

   // Return the value of the data field.
   return field.toBool();
}






/*!
 * Returns the value of this basic block's field with the given id. The given id
 * must exist and it must be a string.
 *
 * @param id The id of this basic block's field whose value is returned.
 *
 * @return Value of this basic block's field with the given id.
 */
QString Block::getString(const QString& id) const
{
   // Get the data field of this basic block with the given id, making sure it is the
   // correct type.
   const QVariant field {get(id)};
   Q_ASSERT(field.type() == QVariant::String);

   // Return the value of the data field.
   return field.toString();
}






/*!
 * Returns the value of this basic block's field with the given id. The given id
 * must exist and it must be a string list.
 *
 * @param id The id of this basic block's field whose value is returned.
 *
 * @return Value of this basic block's field with the given id.
 */
QStringList Block::getStringList(const QString& id) const
{
   // Get the data field of this basic block with the given id, making sure it is the
   // correct type.
   const QVariant field {get(id)};
   Q_ASSERT(field.type() == QVariant::StringList);

   // Return the value of the data field.
   return field.toStringList();
}






/*!
 * Appends a new data field to this basic block. This is only used when
 * initializing a basic block to its type and adding the data field definitions.
 *
 * @param type The type of the data field that is appended to this basic block.
 *
 * @param element The data field element defining the data field that is added
 *                to this basic block.
 *
 * @param isDefault True if the newly appended data field should be set to its
 *                  default value or false if it is set to its null value.
 */
void Block::addField(Field type, const QDomElement& element, bool isDefault)
{
   // Get the id the new data field of this basic block will use, making sure the id
   // attribute exists in the given element.
   const QString key {element.attribute(_idKey)};
   Q_ASSERT(!key.isEmpty());

   // Make sure this basic block does not already have a field with the given id.
   Q_ASSERT(!_fields.contains(key));

   // Add the new field of the given type, setting its default value if the given
   // state indicates to do so or leaving it to its null value if not. Note for
   // string list types there is no default value possible.
   switch (type)
   {
   case Field::Bool:
      if ( isDefault )
      {
         _fields.insert(key,QVariant( element.attribute(_defaultKey).toLower() == QStringLiteral("true") ? true : false));
      }
      else _fields.insert(key,QVariant(false));
      break;
   case Field::String:
      if ( isDefault ) _fields.insert(key,QVariant(element.attribute(_defaultKey)));
      else _fields.insert(key,QVariant(QString()));
      break;
   case Field::StringList:
      _fields.insert(key,QVariant(QStringList()));
      break;
   }
}






/*!
 * Return the data field of this basic block with the given id. A field with the
 * given id must exist.
 *
 * @param id The id of this basic block's data field which is returned.
 *
 * @return Data field of this data block with the given id.
 */
QVariant Block::get(const QString& id) const
{
   // Make sure this basic block has been initialized.
   Q_ASSERT(_type != -1);

   // Find this basic block's data field with the given id, making sure it exists.
   auto i {qAsConst(_fields).find(id)};
   Q_ASSERT(i != _fields.cend());

   // Return the found data field.
   return *i;
}

}
