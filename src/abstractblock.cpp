#include "abstractblock.h"
#include <QDomDocument>
#include <QMap>
#include <exception.h>
#include "abstractblockfactory.h"



using namespace std;
//



/*!
 * The name for version attributes. 
 */
const char* AbstractBlock::_versionTag {"version"};
/*!
 * The tag name for data elements. 
 */
const char* AbstractBlock::_dataTag {"data"};
/*!
 * The name for type attributes. 
 */
const char* AbstractBlock::_typeTag {"type"};






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Pointer to the root block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an abstract block pointer _root_ with it pointing to this abstract 
 *    block. While _root_ has a parent set its pointer to its parent. 
 *
 * 2. Return _root_. 
 */
AbstractBlock* AbstractBlock::root()
{
   // 1
   AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();

   // 2
   return root;
}






/*!
 * This returns the root block in this block's tree structure. The root block is 
 * the common parent of all other blocks that has no parent itself. 
 *
 * @return Read only pointer to the root block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a constant abstract block pointer _root_ with it pointing to this 
 *    abstract block. While _root_ has a parent set its pointer to its parent. 
 *
 * 2. Return _root_. 
 */
const AbstractBlock* AbstractBlock::root() const
{
   // 1
   const AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();

   // 2
   return root;
}






/*!
 * This returns a pointer to the parent of this block. If this block is the root 
 * block then null is returned. 
 *
 * @return If this block is the root then a null pointer, else a pointer to this 
 *         block's parent. 
 */
AbstractBlock* AbstractBlock::parent() const
{
   return qobject_cast<AbstractBlock*>(QObject::parent());
}






/*!
 * This returns the number of children this block contains. 
 *
 * @return Number of children this block contains. 
 */
int AbstractBlock::size() const
{
   return _children.size();
}






/*!
 * This returns a read only reference to this block's list of children blocks. The 
 * list is a list of pointers to all child blocks. 
 *
 * @return Reference to this block's list of children. 
 */
const QList<AbstractBlock*>& AbstractBlock::list() const
{
   return _children;
}






/*!
 * This gets the index where the child with the given pointer is stored in this 
 * block's list of children. If no child with the given pointer is found then -1 is 
 * returned. 
 *
 * @param pointer Pointer of child block to match in this block's list and return 
 *                its index. 
 *
 * @return If a match is found then the index of the child with the given pointer, 
 *         else -1. 
 */
int AbstractBlock::indexOf(AbstractBlock* pointer) const
{
   return _children.indexOf(pointer);
}






/*!
 * This returns a pointer to this block's child with the given index. If the index 
 * is out of range an exception is thrown. 
 *
 * @param index Index of the child whose pointer is returned. 
 *
 * @return Pointer to this block's child with given index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception. 
 *
 * 2. Return a pointer to this block's child with the given index. 
 */
AbstractBlock* AbstractBlock::get(int index) const
{
   // 1
   if ( index < 0 || index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot get child %1 when only %2 children exist.")
                   .arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // 2
   return _children.at(index);
}






/*!
 * Makes an exact copy of this block. This uses other virtual functions to create a 
 * copy of this block. The new block returned has no parent. 
 *
 * @return Exact copy of this block's data as a new block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new empty block _ret_ that is the same type as this block. Copy this 
 *    block's children and implementation data to _ret_. 
 *
 * 2. Return _ret_. 
 */
std::unique_ptr<AbstractBlock> AbstractBlock::makeCopy() const
{
   // 1
   unique_ptr<AbstractBlock> ret {makeBlank()};
   ret->copyChildren(this);
   ret->copyDataFrom(this);

   // 2
   return ret;
}






/*!
 * Tests if this block contains any children of the given type. 
 *
 * @param type The block type to search for within this block's list of children. 
 *
 * @return True if this block contains any children of the given type, else false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through list of this block's children. If a child matches the given 
 *    type then return true. 
 *
 * 2. Return false because no child was found with the given type. 
 */
bool AbstractBlock::containsType(int type) const
{
   // 1
   for (auto child : qAsConst(_children))
   {
      if ( child->type() == type ) return true;
   }

   // 2
   return false;
}






/*!
 * Tests if this block contains any children of any type given. 
 *
 * @param types List of block types to search for within this block's list of 
 *              children. 
 *
 * @return True if this block contains any children of any type given, else false. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through list of this block's children. If a child matches the given 
 *    type then return true. 
 *
 * 2. Return false because no child was found with the given type. 
 */
bool AbstractBlock::containsType(const QList<int>& types) const
{
   // 1
   for (auto child : qAsConst(_children))
   {
      if ( types.contains(child->type()) ) return true;
   }

   // 2
   return false;
}






/*!
 * Sets the value of the field with the given index for this block. If the given 
 * index is out of range then an exception is thrown. 
 *
 * @param index Index of the field whose value is set to the given value. 
 *
 * @param value Value that is set to the field with the given index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception. 
 *
 * 2. If the new given value is different from the current value for the given 
 *    field then call the interface to quietly set it to the new value and then 
 *    call the interface to notify that field has been modified. 
 */
void AbstractBlock::setField(int index, const QVariant& value)
{
   // 1
   if ( index < 0 || index >= fieldSize() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Given block field %1 is out of range (%2 total).")
                   .arg(index)
                   .arg(fieldSize()));
      throw e;
   }

   // 2
   if ( value != field(index) )
   {
      quietlySetField(index,value);
      fieldModified(index);
   }
}






/*!
 * Moves the child block with the given index up the list by one where the very top 
 * of the list is index 0. If the given index is already at the top of the list or 
 * out of range then this does nothing. 
 *
 * @param index The index of the child block moved up by one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is at the top of the list or is out of range then return, 
 *    else swap the child with the given index with the child just above it and 
 *    call the notify modified method. 
 *
 * 2. Starting with this block call its child moved interface and continue calling 
 *    the next parent's child moved interface until the interface returns false or 
 *    the root block is reached. 
 */
void AbstractBlock::moveUp(int index)
{
   // 1
   if ( index < 1 || index >= _children.size() ) return;
   std::swap(_children[index - 1],_children[index]);
   notifyModified();

   // 2
   AbstractBlock* notify {this};
   while ( notify && notify->childMoved(_children.at(index - 1)) ) notify = notify->parent();
}






/*!
 * Moves the child block with the given index down the list by one where the very 
 * top of the list is index 0. If the given index is already at the bottom of the 
 * list or out of range then this does nothing. 
 *
 * @param index The index of the child block moved down by one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is at the top of the list or is out of range then return, 
 *    else swap the child with the given index with the child just below it and 
 *    call the notify modified method. 
 *
 * 2. Starting with this block call its child moved interface and continue calling 
 *    the next parent's child moved interface until the interface returns false or 
 *    the root block is reached. 
 */
void AbstractBlock::moveDown(int index)
{
   // 1
   if ( index < 0 || index >= (_children.size() - 1) ) return;
   std::swap(_children[index],_children[index + 1]);
   notifyModified();

   // 2
   AbstractBlock* notify {this};
   while ( notify && notify->childMoved(_children.at(index + 1)) ) notify = notify->parent();
}






/*!
 * Inserts a new child block into this block's list of children at the given index. 
 * If the given index is less than 0 it is prepended to this block's list, else if 
 * the given index is greater than this block's list size then it is appended. If 
 * the given pointer to the new block is null or it is a type this block cannot 
 * contain then an exception is thrown. 
 *
 * @param index The index where the new block is inserted. 
 *
 * @param child Pointer to the new block that is inserted as this block's child. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given child pointer is null or its type is not contained in this 
 *    blocks build list then throw an exception. 
 *
 * 2. Insert the new block into this block's list of children at the given index, 
 *    releasing it from its smart pointer and setting this block as its parent. 
 *    Call the notify modified method. 
 *
 * 3. Starting with this block call its child added interface and continue calling 
 *    the next parent's child added interface until the interface returns false or 
 *    the root block is reached. 
 */
void AbstractBlock::insert(int index, std::unique_ptr<AbstractBlock>&& child)
{
   // 1
   if ( !child )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block with null pointer."));
      throw e;
   }
   if ( !buildList().contains(child->type()) )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot insert child block of type %1 to parent block of type %2.")
                   .arg(child->type())
                   .arg(type()));
      throw e;
   }

   // 2
   AbstractBlock* adopted {child.release()};
   adopted->setParent(this,index);
   notifyModified();

   // 3
   AbstractBlock* notify {this};
   while ( notify && notify->childAdded(adopted) ) notify = notify->parent();
}






/*!
 * Takes the child block at the given index this block contains. The child block 
 * returned no longer has a parent. If the index is out of range then an exception 
 * is thrown. 
 *
 * @param index The index of the child that is taken. 
 *
 * @return Pointer to child that was taken from this block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is out of range then throw an exception. 
 *
 * 2. Remove the child at the given index from this block's list of children saving 
 *    its pointer to the smart pointer _ret_. Call the child removed interface and 
 *    call the notify modified method. 
 *
 * 3. Starting with this block call its child removed interface with _ret_ and 
 *    continue calling the next parent's child removed interface until the 
 *    interface returns false or the root block is reached. 
 *
 * 4. Return _ret_. 
 */
std::unique_ptr<AbstractBlock> AbstractBlock::take(int index)
{
   // 1
   if ( index < 0 && index >= _children.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot take child %1 when only %2 children exist.")
                   .arg(index)
                   .arg(_children.size()));
      throw e;
   }

   // 2
   unique_ptr<AbstractBlock> ret {_children.at(index)};
   ret->setParent(nullptr);
   notifyModified();

   // 3
   AbstractBlock* notify {this};
   while ( notify && notify->childRemoved(ret.get()) ) notify = notify->parent();

   // 4
   return ret;
}






/*!
 * Removes the child block at the given index, deleting it and any children it may 
 * contain. If the given index is out of range then an exception is thrown. 
 *
 * @param index The index of the child that is removed and deleted. 
 */
void AbstractBlock::remove(int index)
{
   take(index).reset();
}






/*!
 * Reads in block data from an XML element. This clears any existing data this 
 * block contains, including the removal and deletion of all existing children. 
 * This recursively adds any children contained in the XML reading in their data 
 * from child XML elements. 
 *
 * @param element The XML element that stores block data this block reads as input. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete and clear any existing children this block may contain. 
 *
 * 2. Iterate through all children nodes of the given element. If a child is an 
 *    element and matches the data tag then read it in as this block's new data, 
 *    else if it an element that read it in as a new child block. 
 *
 * 3. Make sure all new children of this block is allowed to this block's child 
 *    using its built list interface. If any child is found that is not allowed 
 *    then throw an exception. 
 */
void AbstractBlock::read(const QDomElement& element)
{
   // 1
   qDeleteAll(_children);
   _children.clear();

   // 2
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         if ( element.tagName() == _dataTag )
         {
            readData(element);
            break;
         }
         else readChild(element);
      }
   }

   // 3
   for (auto child: qAsConst(_children))
   {
      if ( !buildList().contains(child->type()) )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Loaded illegal child block type '%1' to block type '%2'")
                      .arg(factory().name(child->type()))
                      .arg(factory().name(type())));
         throw e;
      }
   }
}






/*!
 * Writes out this block's data as an XML element. This includes all children as 
 * child XML elements which have their write functions called recursively. 
 *
 * @param document  
 *
 * @return XML element that stores this block's data and all children underneath 
 *         it. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new XML element _data_ writing all of this block's data to it. 
 *
 * 2. Create a new XML element _ret_. Adding this block's type to _ret_ as an 
 *    attribute and appending the _data_ element. Iterate through this block's list 
 *    of children, calling their write method and appending the returned child 
 *    element to _ret_. 
 *
 * 3. Return _ret_. 
 */
QDomElement AbstractBlock::write(QDomDocument& document) const
{
   // 1
   QDomElement data {writeData(document)};

   // 2
   QDomElement ret {document.createElement(factory().elementName(type()))};
   ret.setAttribute(_typeTag,type());
   ret.appendChild(data);
   for (auto child : _children) ret.appendChild(child->write(document));

   // 3
   return ret;
}






/*!
 * This interface is called whenever a child below this block has modified its name 
 * and keeps calling this interface the next block parent until this returns false. 
 * The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that has modified its name. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childNameModified(AbstractBlock* child)
{
   Q_UNUSED(child)
   return false;
}






/*!
 * This interface is called whenever a new child below this block has been added 
 * and keeps calling this interface on the next block parent until this returns 
 * false. The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that been added to its new parent block. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   return false;
}






/*!
 * This interface is called whenever an existing child below this block has been 
 * removed and keeps calling this interface on the next block parent until this 
 * returns false. The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that has been removed from its former 
 *              parent block. This object can be deleted right after this call. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   return false;
}






/*!
 * This interface is called whenever an existing child below this block has been 
 * moved and keeps calling this interface on the next block parent until this 
 * returns false. The default implementation does nothing and returns false. 
 *
 * @param child Pointer to the child block that has been moved in its parent's list 
 *              of children. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool AbstractBlock::childMoved(AbstractBlock* child)
{
   Q_UNUSED(child)
   return false;
}






/*!
 * Returns the data version of the data this block is reading in. If this block is 
 * not reading in data then this is the current version for this block's data. 
 *
 * @return Data version of the data this block is reading in or the current version 
 *         if data is not being read in. 
 */
int AbstractBlock::dataVersion() const
{
   return _version;
}






/*!
 * Notifies this block has been modified by finding its root block and emitting its 
 * modified signal. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Find the root block of this block and emit its modified signal. 
 */
void AbstractBlock::notifyModified()
{
   // 1
   AbstractBlock* root {this};
   while ( root->parent() ) root = root->parent();
   emit root->modified();
}






/*!
 * Notifies this block's name has been modified by finding its root block and 
 * emitting its name modified signal. If this is called by a root block an 
 * exception is thrown because the root has no name. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a pointer _root_ to the parent of this block. If the returned pointer 
 *    is null then throw an exception. 
 *
 * 2. Starting with _root_ call its child name modified interface and continue 
 *    calling the next parent's child name modified interface until the interface 
 *    returns false or the root block is reached. 
 *
 * 3. Find the root block of _root_ and emit its name modified signal with this 
 *    block's pointer. 
 */
void AbstractBlock::notifyNameModified()
{
   // 1
   AbstractBlock* root {parent()};
   if ( !root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot notify a name change of the root block."));
      throw e;
   }

   // 2
   AbstractBlock* notify {root};
   while ( notify && notify->childNameModified(this) ) notify = notify->parent();

   // 3
   while ( root->parent() ) root = root->parent();
   emit root->nameModified(this);
}






/*!
 * Notifies this block's body has been modified by finding its root block and 
 * emitting its name modified signal. If this is called by a root block an 
 * exception is thrown because the root has no body. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a pointer _root_ to the parent of this block. If the returned pointer 
 *    is null then throw an exception. 
 *
 * 2. Find the root block of _root_ and emit its body modified signal with this 
 *    block's pointer. 
 */
void AbstractBlock::notifyBodyModified()
{
   // 1
   AbstractBlock* root {parent()};
   if ( !root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot notify a body change of the root block."));
      throw e;
   }

   // 2
   while ( root->parent() ) root = root->parent();
   emit root->bodyModified(this);
}






/*!
 * Reads in this block's field data from the given XML element. This overwrites any 
 * value this block's fields may already contain. 
 *
 * @param element The XML element that is read in as this block's data. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this block's data version number to the version attribute from the given 
 *    XML element. 
 *
 * 2. Create a mapping _lists_ of all fields that are the string list type, setting 
 *    each mapping to a blank string list. 
 *
 * 3. Iterate through all children nodes of the given XML element. If a node is an 
 *    element and matches a field tag then quietly set the fields value. If the 
 *    field type is boolean then simply set it to true, else if it is string then 
 *    set it to the element's text, else if it is a string list then append the 
 *    element's text to the correct string list mapping in _lists_. 
 *
 * 4. Iterate through all string list field mappings in _lists_, quietly setting 
 *    each field with the correct string list. 
 *
 * 5. Set this block's data version to its implementation's current version. 
 */
void AbstractBlock::readData(const QDomElement& element)
{
   // 1
   _version = element.attribute(_versionTag,"0").toInt();

   // 2
   QMap<int,QStringList> lists;
   for (int i = 0; i < fieldSize() ;++i)
   {
      if ( fieldType(i) == StringList ) lists.insert(i,QStringList());
   }

   // 3
   QDomNode node {element.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         int i {fieldIndexOf(element.tagName())};
         if ( i >= 0 )
         {
            switch (fieldType(i))
            {
            case Boolean:
               quietlySetField(i,true);
               break;
            case String:
               quietlySetField(i,element.text());
               break;
            case StringList:
               lists[i] << element.text();
               break;
            }
         }
      }
   }

   // 4
   for ( auto i = lists.begin(); i != lists.end() ;++i)
   {
      quietlySetField(i.key(),*i);
   }

   // 5
   _version = version();
}






/*!
 * Writes out this block's field data to the returned XML element, using the given 
 * XML document to create the returned element. 
 *
 * @param document XML document used for creating the returned data element. 
 *
 * @return XML element containing all this block's field data. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new XML element _ret_, setting its tag name to the data tag and 
 *    appending its implementation's current data version as an attribute. 
 *
 * 2. Iterate through all fields for this block. For each field add their data as a 
 *    child element to _ret_ using the field tag name for each one. If the field 
 *    type is boolean simply add an empty element if its value is true. If the 
 *    field type is string add an element whose text is the field string if it is 
 *    not empty. If the field type is string list then add an element for each 
 *    string the list contains. 
 *
 * 3. Return _ret_. 
 */
QDomElement AbstractBlock::writeData(QDomDocument& document) const
{
   // 1
   QDomElement ret {document.createElement(_dataTag)};
   ret.setAttribute(_versionTag,QString::number(version()));

   // 2
   for (int i = 0; i < fieldSize() ;++i)
   {
      switch (fieldType(i))
      {
      case Boolean:
         if ( field(i).toBool() ) ret.appendChild(document.createElement(fieldTag(i)));
         break;
      case String:
         {
            QString value {field(i).toString()};
            if ( !value.isEmpty() )
            {
               QDomElement element {document.createElement(fieldTag(i))};
               element.appendChild(document.createTextNode(value));
               ret.appendChild(element);
            }
            break;
         }
      case StringList:
         {
            for (auto string: field(i).toStringList())
            {
               QDomElement element {document.createElement(fieldTag(i))};
               element.appendChild(document.createTextNode(string));
               ret.appendChild(element);
            }
            break;
         }
      }
   }

   // 3
   return ret;
}






/*!
 * Copies all children from the given block and appends the copies to the list of 
 * this block's children. This does not remove any previous children already part 
 * of this block. 
 *
 * @param parent Pointer to block whose children are copied. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all the children of the given block. Make a copy of each 
 *    child, appending them to this block's list of children. 
 */
void AbstractBlock::copyChildren(const AbstractBlock* parent)
{
   // 1
   for (auto child : qAsConst(parent->_children))
   {
      child->makeCopy().release()->setParent(this,size());
   }
}






/*!
 * Copies all field data from the given block to this one, overwriting all field 
 * data this block currently contains. This assumes the given block is the same 
 * type as this one. 
 *
 * @param other  
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all fields for this block, for each one setting its value to 
 *    the value of the other block's corresponding field. 
 */
void AbstractBlock::copyDataFrom(const AbstractBlock* other)
{
   // 1
   for (int i = 0; i < fieldSize() ;++i)
   {
      quietlySetField(i,other->field(i));
   }
}






/*!
 * Creates a new child reading it in from the given XML element. The new child is 
 * then appended to this block's list of children. 
 *
 * @param element XML element used to read in the new child. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in the type attribute from the given child element to _type_. If reading 
 *    the type as an integer fails or the type is out of range of possible block 
 *    types for this project type then throw an exception. 
 *
 * 2. Create a new block with the given type and call its read method with the 
 *    given child XML element, appending it to this blocks list of children. 
 */
void AbstractBlock::readChild(const QDomElement& element)
{
   // 1
   bool ok;
   int type {element.attribute(_typeTag,"nan").toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Type attribute is not an integer."));
      throw e;
   }
   if ( type < 0 || type >= factory().size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Read in invalid type %1 when max is %2.").arg(type).arg(factory().size()));
      throw e;
   }

   // 3
   unique_ptr<AbstractBlock> child {factory().makeBlock(type)};
   child->read(element);
   child.release()->setParent(this,size());
}






/*!
 * Sets this block's parent to the given block, adding it to its new parent's list 
 * with the given index. If the index is less than 1 it is prepended, else if it is 
 * greater than the size of the list it is appended. This will remove any previous 
 * parent this block may have had. The new parent can be a null pointer which means 
 * this block will have no parent. 
 *
 * @param parent Pointer to the new parent for this block. 
 *
 * @param index The index where this block will be added to its new parent's list. 
 */
void AbstractBlock::setParent(AbstractBlock* parent, int index)
{
   // 1
   if ( AbstractBlock* oldParent = AbstractBlock::parent() )
   {
      oldParent->_children.removeOne(this);
   }

   // 2
   QObject::setParent(parent);
   if ( parent )
   {
      parent->_children.insert(index,this);
   }
}
