#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>
#include <QObject>
#include <QDomElement>
#include <exception.h>
#include "global.h"
#include "gui.h"
//



/*!
 * This represents a single block of data in the reference system. It conforms 
 * to a parent child relationship where each block can have any number of 
 * children that are also blocks. The children are structured in an ordered list 
 * so they can be represented as a tree like structure in a view. Blocks also 
 * spawn the GUI elements that represent their models. Copying of blocks is also 
 * performed with other virtual interface functions. Many helper functions are 
 * available to navigate the tree structure of children and parents. Blocks are 
 * identified by a type which is an integer value that must be unique among all 
 * block types of a project type supplied by the block factory. Blocks are saved 
 * and opened using XML elements. The data for these XML elements have version 
 * numbers so later versions of this software can identify and read previous 
 * versions of XML elements. 
 */
class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface returns this block's type. 
    *
    * @return This block's type. 
    */
   virtual int type() const = 0;
   /*!
    * This interface returns a reference to this block's factory which produces all 
    * block types for this project type. 
    *
    * @return Reference to block factory. 
    */
   virtual const AbstractBlockFactory& factory() const = 0;
   /*!
    * This interface returns the name of this block. The name is used to display 
    * the block as text in a tree view and as a title when selected. 
    *
    * @return The name of this block. 
    */
   virtual QString name() const = 0;
   /*!
    * This interface returns the icon of this block. The icon is displayed in a 
    * tree view of this block and its title if selected. 
    *
    * @return The icon of this block. 
    */
   virtual QIcon icon() const = 0;
   /*!
    * This interface returns a list of types that this block can contain as 
    * children. If the same type is listed more than once any repeated occurrences 
    * are ignored and the type is allowed to be this block's parent. 
    *
    * @return List of allowed types this block can contain as children. 
    */
   virtual QList<int> buildList() const = 0;
   /*!
    * This interface returns a View that provides a detailed read only GUI 
    * representation of this block's data. 
    *
    * @return New GUI view that represents this block's data. 
    */
   virtual std::unique_ptr<QWidget> makeView() const = 0;
   /*!
    * This interface returns a editable GUI widget that provides the ability to 
    * edit this block's data. 
    *
    * @return New editable GUI widget to edit this block's data. 
    */
   virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() = 0;
   virtual std::unique_ptr<AbstractBlock> makeCopy() const;
   AbstractBlock* root();
   const AbstractBlock* root() const;
   AbstractBlock* parent() const;
   int size() const;
   const QList<AbstractBlock*>& list() const;
   int indexOf(AbstractBlock* pointer) const;
   AbstractBlock* get(int index) const;
   bool containsType(int type) const;
   bool containsType(const QList<int>& types) const;
   template<class T> QList<T*> makeListOfType(int type) const;
   template<class T> const T* cast(int toType) const;
   template<class T> T* cast(int toType);
   void moveUp(int index);
   void moveDown(int index);
   void insert(int index, std::unique_ptr<AbstractBlock>&& child);
   std::unique_ptr<AbstractBlock> take(int index);
   void remove(int index);
   void read(const QDomElement& element);
   QDomElement write(QDomDocument& document) const;
signals:
   /*!
    * Signals that this block has been modified and should set the project as 
    * modified. 
    */
   void modified();
   /*!
    * Signals that a child with the given pointer has changed its name. This child 
    * could be any depth below this block. 
    *
    * @param child Pointer of the child whose name has changed. 
    */
   void nameChanged(AbstractBlock* child);
protected slots:
   virtual void childNameChanged(AbstractBlock* child);
   virtual void childAdded(AbstractBlock* child);
   virtual void childRemoved(AbstractBlock* child);
   virtual void childMoved(AbstractBlock* child);
protected:
   /*!
    * This interface reads in the data for this block from the given XML element 
    * and version number. 
    *
    * @param element The XML element used to read in this blocks data. 
    *
    * @param version The version of the data stored in the XML. 
    */
   virtual void readData(const QDomElement& element, int version) = 0;
   /*!
    * This interface returns the current version number of XML elements written for 
    * this block type. 
    *
    * @return Current version number. 
    */
   virtual int writeVersion() const = 0;
   /*!
    * This interface returns a XML element containing the data for this block using 
    * the current version number. Attributes should never be used with this element 
    * because the block system already uses one for the version number. 
    *
    * @param document XML document to use for creating new elements. 
    *
    * @return XML element containing the data of this block. 
    */
   virtual QDomElement writeData(QDomDocument& document) const = 0;
   /*!
    * This interface makes a new block object of this block's type with no data and 
    * returns a pointer to the new block. 
    *
    * @return Pointer to the newly created block. 
    */
   virtual std::unique_ptr<AbstractBlock> makeBlank() const = 0;
   /*!
    * This interface copies all data from the given block to this block, 
    * overwriting any data this block may already contain. This does not copy any 
    * children. 
    *
    * @param other The other block whose data will be copied. 
    */
   virtual void copyDataFrom(const AbstractBlock* other) = 0;
   static QDomElement makeElement(QDomDocument& document, const QString& tagName, const QString& text);
   void notifyOfNameChange();
private slots:
   void childModified();
   void childNameModified(AbstractBlock* child);
private:
   void copyChildren(const AbstractBlock* parent);
   void setParent(AbstractBlock* parent, int index = -1);
   void readChild(const QDomElement& element);
   /*!
    * This stores the name for version attributes. 
    */
   static const char* _versionTag;
   /*!
    * This stores the tag name for data elements. 
    */
   static const char* _dataTag;
   /*!
    * This stores the name for type attributes. 
    */
   static const char* _typeTag;
   /*!
    * This stores the pointer list of this block's children. 
    */
   QList<AbstractBlock*> _children;
};






/*!
 * Build a list of this node's children that matches the given type. The 
 * returned list has no other copies. 
 *
 * @tparam T The child class type that is matched. 
 *
 * @param type The type whose matches are added to the list. 
 *
 * @return List of this node's children that is given type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through the list of this node's children. 
 *
 * 2. Add a pointer of every child that matches the given type to the return 
 *    list. 
 *
 * 3. Return the list of matched children. 
 */
template<class T> QList<T*> AbstractBlock::makeListOfType(int type) const
{
   QList<T*> ret;
   for (auto child : list())
   {
      if ( T* variable = child->cast<T>(type) ) ret.append(variable);
   }
   return ret;
}






/*!
 * This casts this block to a specific block type. If the given type does not 
 * match this block's type then a null pointer is returned. If this object fails 
 * to cast as the requested type an exception is thrown. 
 *
 * @tparam T The class type that is cast. 
 *
 * @param toType The block type that this block is cast to. 
 *
 * @return If successful then a read only pointer of requested cast type, else a 
 *         null pointer. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given type to cast does not match this block's type then return a 
 *    null pointer, else go to the next step. 
 *
 * 2. Cast this block's pointer to the requested class type and return the cast 
 *    pointer. If the cast fails then throw an exception. 
 */
template<class T> const T* AbstractBlock::cast(int toType) const
{
   if ( type() == toType )
   {
      if ( const T* ret = qobject_cast<const T*>(this) ) return ret;
      else
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Failed casting object to required type."));
         throw e;
      }
   }
   return nullptr;
}






/*!
 * This casts this block to a specific block type. If the given type does not 
 * match this block's type then a null pointer is returned. If this object fails 
 * to cast as the requested type an exception is thrown. 
 *
 * @tparam T The class type that is cast. 
 *
 * @param toType The block type that this block is cast to. 
 *
 * @return If successful then a pointer of requested cast type, else a null 
 *         pointer. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given type to cast does not match this block's type then return a 
 *    null pointer, else go to the next step. 
 *
 * 2. Cast this block's pointer to the requested class type and return the cast 
 *    pointer. If the cast fails then throw an exception. 
 */
template<class T> T* AbstractBlock::cast(int toType)
{
   if ( type() == toType )
   {
      if ( T* ret = qobject_cast<T*>(this) ) return ret;
      else
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Failed casting object to required type."));
         throw e;
      }
   }
   return nullptr;
}



#endif
