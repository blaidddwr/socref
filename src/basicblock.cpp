#include "basicblock.h"
#include <QIcon>
#include <QJsonObject>
#include <socutil/sut_exceptions.h>
#include "basicblock_edit.h"
#include "basicblock_view.h"



using namespace Sut;
//



/*!
 * The boolean tag name of elements that define a boolean data field. 
 */
const char* BasicBlock::_boolTag {"bool"};
/*!
 * The string tag name of elements that define a string data field. 
 */
const char* BasicBlock::_stringTag {"string"};
/*!
 * The string list tag name of elements that define a string list data field. 
 */
const char* BasicBlock::_stringListTag {"stringlist"};
/*!
 * The tag name of the element used to define the name option that specifies which 
 * string field is used for this block's name. 
 */
const char* BasicBlock::_nameTag {"name"};
/*!
 * The tag name of the element used to define the path for this basic block type's 
 * icon. 
 */
const char* BasicBlock::_iconTag {"icon"};
/*!
 * The tag name of the element that hold the configuration for this basic block 
 * type's edit class. 
 */
const char* BasicBlock::_editTag {"edit"};
/*!
 * The tag name of the element that hold the configuration for this basic block 
 * type's view class. 
 */
const char* BasicBlock::_viewTag {"view"};
/*!
 * The attribute name for the id of data field element definitions. 
 */
const char* BasicBlock::_idKey {"id"};
/*!
 * The attribute name for default values of data field element definitions. 
 */
const char* BasicBlock::_defaultKey {"default"};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int BasicBlock::type() const
{
   // Make sure this basic block has been initialized. 
   check();

   // Return the block type of this basic block. 
   return _type;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
const AbstractBlockFactory& BasicBlock::factory() const
{
   // Make sure this basic block has been initialized. 
   check();

   // Return a reference to this basic block's factory. 
   return *_factory;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString BasicBlock::name() const
{
   // Make sure this basic block has been initialized. 
   check();

   // Make sure this basic block's name field id is not empty. 
   if ( _nameFieldId.isEmpty() ) return QString();

   // Get this basic block's field with its name field id, making it exists and is 
   // the correct field type. 
   const QVariant field {_fields.value(_nameFieldId)};
   if ( field.type() != QVariant::String )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Name field id '%1' points to invalid field.").arg(_nameFieldId));
      throw e;
   }

   // Return the found string field. 
   return field.toString();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon BasicBlock::icon() const
{
   // Make sure this basic block has been initialized. 
   check();

   // Return this basic block type's icon. 
   return QIcon(_iconPath);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> BasicBlock::buildList() const
{
   // Make sure this basic block has been initialized. 
   check();

   // Return this basic block type's build list. 
   return _buildList;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<QWidget> BasicBlock::makeView() const
{
   QPtr<BasicBlock::View> ret(makeBasicView());
   ret->update();
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<Gui::AbstractEdit> BasicBlock::makeEdit()
{
   return new Edit(this);
}






/*!
 * Initializes this basic block to the given type. This must be called before this 
 * basic block can be used and cannot be called again. 
 *
 * @param type The block type of this new basic block. 
 *
 * @param factory The basic block factory that made this new basic block. 
 *
 * @param element The XML element that defines all data fields and options of the 
 *                basic block type this new basic block is being initialized to. 
 *
 * @param buildList The build list of the basic block type this new basic block is 
 *                  being initialized to. 
 *
 * @param isDefault True if the data fields should be set to default values or 
 *                  false to set them to their null values. 
 */
void BasicBlock::initialize(int type, const AbstractBlockFactory* factory, const QDomElement& element, const QList<int>& buildList, bool isDefault)
{
   // Make sure this basic block has NOT been initialized. 
   if ( _type != -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to initialize basic block object that is already initialized."));
      throw e;
   }

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
 * block. The default interface returns a generic basic block view. This should be 
 * used for basic block views that use the custom view element in its definition. 
 *
 * @return New basic block view implementation for this basic block. 
 */
Sut::QPtr<BasicBlock::View> BasicBlock::makeBasicView() const
{
   return new View(this);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param element See interface docs. 
 */
void BasicBlock::readData(const QDomElement& element)
{
   // Make sure this basic block has been initialized. 
   check();

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
               Exception::LogicError e;
               SUT_MARK_EXCEPTION(e);
               e.setDetails(tr("Encountered basic block field of unsupported type."));
               throw e;
            }
         }
      }

      // Move to the next sibling node. 
      node = node.nextSibling();
   }
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param document See interface docs. 
 *
 * @return See interface docs. 
 */
QDomElement BasicBlock::writeData(QDomDocument& document) const
{
   // Make sure this basic block has been initialized. 
   check();

   // Create the return data element. 
   QDomElement ret {document.createElement("na")};

   // Iterate through all data fields of this basic block. 
   for (auto i = _fields.cbegin(); i != _fields.cend() ;++i)
   {
      // Based off the data field type write out its data as child elements appended to 
      // the data element to be returned, throwing an exception if the field is an 
      // unknown type. 
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

   // Return the complete data element that stores this basic block's field data. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> BasicBlock::makeBlank() const
{
   // Make sure this basic block has been initialized. 
   check();

   // Create a new uninitialized basic block, making sure it worked. 
   QPtr<BasicBlock> ret {qobject_cast<BasicBlock*>(metaObject()->newInstance())};
   if ( !ret )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Failed making new instance of basic block type %1.")
                   .arg(metaObject()->className()));
      throw e;
   }

   // Initialize the new basic block to this basic block's type. 
   ret->initialize(_type,_factory,_definition,_buildList,false);

   // Return the initialized basic block. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param other See interface docs. 
 */
void BasicBlock::copyDataFrom(const AbstractBlock* other)
{
   // Make sure this basic block has been initialized. 
   check();

   // Cast the given abstract block to a basic block, making sure it worked. 
   const BasicBlock* basic {qobject_cast<const BasicBlock*>(other)};
   if ( !basic )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Given abstract block to copy is not a basic block."));
      throw e;
   }

   // Iterate through all the fields of the given basic block. 
   for (auto i = basic->_fields.cbegin(); i != basic->_fields.cend() ;++i)
   {
      // Find the corresponding field of this basic block, making sure it was found. 
      auto j {_fields.find(i.key())};
      if ( j == _fields.end() )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Missing field to copy from given basic block."));
         throw e;
      }

      // Make sure the field types matched between this and the other basic block. 
      if ( i->type() != j->type() )
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Field type mismatch with given basic block."));
         throw e;
      }

      // Copy the value of the given basic block field to this basic block. 
      *j = *i;
   }
}






/*!
 * This interface returns a JSON object of available variable types within the 
 * scope of this basic block. This is used for project types that use hardly typed 
 * variables such as C++. For project types like PHP that use soft variables this 
 * can returns an empty object. This is called on a specific block instance because 
 * the available types for selection can depend on the scope which should be 
 * determined by the variable's parent block. There can be additional JSON objects 
 * nested within the root which provides additional types for selection. Each key 
 * in the JSON object is used as a possible type for selection. The default 
 * implementation returns an empty object, assuming a softly typed project type. 
 *
 * @return List of all possible variable types, with possible nested lists, for 
 *         hardly typed project types or an empty object for softly typed project 
 *         types. 
 */
QJsonObject BasicBlock::typeList() const
{
   return QJsonObject();
}






/*!
 * Returns the value of this basic block's field with the given id. If the given id 
 * does not exist or is not a boolean then an exception is thrown. 
 *
 * @param id The id of this basic block's field whose value is returned. 
 *
 * @return Value of this basic block's field with the given id. 
 */
bool BasicBlock::getBool(const QString& id) const
{
   // Get the data field of this basic block with the given id, making sure it is the 
   // correct type. 
   const QVariant field {get(id)};
   if ( field.type() != QVariant::Bool )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' is not a boolean."));
      throw e;
   }

   // Return the value of the data field. 
   return field.toBool();
}






/*!
 * Returns the value of this basic block's field with the given id. If the given id 
 * does not exist or is not a string then an exception is thrown. 
 *
 * @param id The id of this basic block's field whose value is returned. 
 *
 * @return Value of this basic block's field with the given id. 
 */
QString BasicBlock::getString(const QString& id) const
{
   // Get the data field of this basic block with the given id, making sure it is the 
   // correct type. 
   const QVariant field {get(id)};
   if ( field.type() != QVariant::String )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' is not a string."));
      throw e;
   }

   // Return the value of the data field. 
   return field.toString();
}






/*!
 * Returns the value of this basic block's field with the given id. If the given id 
 * does not exist or is not a string list then an exception is thrown. 
 *
 * @param id The id of this basic block's field whose value is returned. 
 *
 * @return Value of this basic block's field with the given id. 
 */
QStringList BasicBlock::getStringList(const QString& id) const
{
   // Get the data field of this basic block with the given id, making sure it is the 
   // correct type. 
   const QVariant field {get(id)};
   if ( field.type() != QVariant::StringList )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' is not a string list."));
      throw e;
   }

   // Return the value of the data field. 
   return field.toStringList();
}






/*!
 * Checks to make sure this basic block has been initialized, throwing an exception 
 * if it is uninitialized. 
 */
void BasicBlock::check() const
{
   // Make sure this basic block has been initialized. 
   if ( _type == -1 )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Attempting to use uninitialized basic block object."));
      throw e;
   }
}






/*!
 * Appends a new data field to this basic block. This is only used when 
 * initializing a basic block to its type and adding the data field definitions. 
 *
 * @param type The type of the data field that is appended to this basic block. 
 *
 * @param element The data field element defining the data field that is added to 
 *                this basic block. 
 *
 * @param isDefault True if the newly appended data field should be set to its 
 *                  default value or false if it is set to its null value. 
 */
void BasicBlock::addField(Field type, const QDomElement& element, bool isDefault)
{
   // Get the id the new data field of this basic block will use, making sure the id 
   // attribute exists in the given element. 
   const QString key {element.attribute(_idKey)};
   if ( key.isEmpty() )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Field definition element %1 missing id attribute.").arg(element.tagName()));
      throw e;
   }

   // Make sure this basic block does not already have a field with the given id. 
   if ( _fields.contains(key) )
   {
      Exception::ReadError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Field definition element %1 redefining id '%2'.")
                   .arg(element.tagName())
                   .arg(key));
      throw e;
   }

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
 * Return the data field of this basic block with the given id. If no field exists 
 * with the given id then an exception is thrown. 
 *
 * @param id The id of this basic block's data field which is returned. 
 *
 * @return Data field of this data block with the given id. 
 */
QVariant BasicBlock::get(const QString& id) const
{
   // Make sure this basic block has been initialized. 
   check();

   // Find this basic block's data field with the given id, making sure it exists. 
   auto i {qAsConst(_fields).find(id)};
   if ( i == _fields.cend() )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("The given field id '%1' does not exist.").arg(id));
      throw e;
   }

   // Return the found data field. 
   return *i;
}
