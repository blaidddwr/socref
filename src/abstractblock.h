#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>
#include <QObject>
#include <QDomElement>
#include <QVariant>
#include <exception.h>
#include "global.h"
#include "gui.h"
//



/*!
 * This represents a single block in this application's reference system. The block 
 * system is the core function of this application, representing blocks as a single 
 * atomic item for referencing code. The block system conforms to a parent child 
 * relationship. Virtual interfaces are provided for basic information, spawning 
 * view and abstract edit widgets, and defining all field data of the block. Using 
 * the field data interface this class can make copies of any implementation type 
 * along with saving/writing the blocks. 
 * 
 * The block system represents all elements of code structure in a reference. 
 * Individual implementations should be defined for different code objects such as 
 * variables or functions. They should also be defined for modifiers or additions 
 * to other blocks, such as arguments to a function or methods to a class. 
 * 
 * Each block can have a single parent and any number of children. The children are 
 * contained in a ordered list to allow the entire block structure to be viewed in 
 * this application's block view widget. This allows for easy mimicking of most 
 * code structures that also follow this relationship, such as arguments of a 
 * function or methods of a class. 
 * 
 * The virtual interfaces for this class fall into three primary groupings. The 
 * first provides the system with basic information usually required to display the 
 * block in the block view. The second deals with spawning view and abstract edit 
 * widgets to have a detailed view of the block or edit its data, respectively. The 
 * final deals with making the data of the block visible to the application by 
 * defining a list of fields. Optional interfaces are also provides for dealing 
 * with child events, such as children being added or removed. 
 * 
 * This class itself handles all copying, reading, and writing of blocks by using 
 * the fields interface which should provide it with all data for an 
 * implementation. This class also handles all management of children internally. 
 * As a result an implementation of this class only needs to provide information 
 * about itself, letting this abstract class take care its management. 
 */
class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   /*!
    * Defines the different types of data for a field. 
    */
   enum Field
   {
      /*!
       * Defines the boolean field type. This type is a simply true or false. If this 
       * field type is never set this it should be false by default. 
       */
      Boolean
      /*!
       * Defines the string field type. 
       */
      ,String
      /*!
       * Defines the string list field type. 
       */
      ,StringList
   };
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
    * This interface returns the name of this block. The name is used to display the 
    * block as text in a tree view and as a title when selected. 
    *
    * @return The name of this block. 
    */
   virtual QString name() const = 0;
   /*!
    * This interface returns the icon of this block. The icon is displayed in a tree 
    * view of this block and its title if selected. 
    *
    * @return The icon of this block. 
    */
   virtual QIcon icon() const = 0;
   /*!
    * This interface returns a list of types that this block can contain as children. 
    * If the same type is listed more than once any repeated occurrences are ignored 
    * and the type is allowed to be this block's parent. 
    *
    * @return List of allowed types this block can contain as children. 
    */
   virtual QList<int> buildList() const = 0;
   /*!
    * This interface returns a view that provides a detailed read only GUI 
    * representation of this block's data. 
    *
    * @return New GUI view that represents this block's data. 
    */
   virtual std::unique_ptr<QWidget> makeView() const = 0;
   /*!
    * This interface returns the number of fields this block contains. 
    *
    * @return The number of fields this object contains. 
    */
   virtual int fieldSize() const = 0;
   /*!
    * This interface returns the field type for the given field index of this block. 
    *
    * @param index Index of the field whose field type is returned. 
    *
    * @return Field type of the given field index of this block. 
    */
   virtual Field fieldType(int index) const = 0;
   /*!
    * This interface returns the value of the field with the given index for this 
    * block. 
    *
    * @param index Index of the field whose value is returned. 
    *
    * @return Value of the field with the given index for this block. 
    */
   virtual QVariant field(int index) const = 0;
   /*!
    * This interface returns a editable GUI widget that provides the ability to edit 
    * this block's data. 
    *
    * @return New editable GUI widget to edit this block's data. 
    */
   virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() = 0;
public:
   AbstractBlock* root();
   const AbstractBlock* root() const;
   AbstractBlock* parent() const;
   int size() const;
   const QList<AbstractBlock*>& list() const;
   int indexOf(AbstractBlock* pointer) const;
   AbstractBlock* get(int index) const;
   std::unique_ptr<AbstractBlock> makeCopy() const;
   bool containsType(int type) const;
   bool containsType(const QList<int>& types) const;
   template<class T> QList<T*> makeListOfType(int type) const;
   template<class T> const T* cast(int toType) const;
   template<class T> T* cast(int toType);
   void setField(int index, const QVariant& value);
   void moveUp(int index);
   void moveDown(int index);
   void insert(int index, std::unique_ptr<AbstractBlock>&& child);
   std::unique_ptr<AbstractBlock> take(int index);
   void remove(int index);
   void read(const QDomElement& element);
   QDomElement write(QDomDocument& document) const;
signals:
   /*!
    * Signals that a child block of this block has been modified. The given child 
    * block could be any depth below this block. Only the root block emits this 
    * signal. 
    */
   void modified();
   /*!
    * Signals that a child block of this block with the given pointer has modified its 
    * name. The given child block could be any depth below this block. Only the root 
    * block emits this signal. 
    *
    * @param child Pointer of the child whose name has changed. 
    */
   void nameModified(AbstractBlock* child);
   /*!
    * Signals that a child block of this block with the given pointer has modified its 
    * body. The given child block could be any depth below this block. Only the root 
    * block emits this signal. 
    *
    * @param child  
    */
   void bodyModified(AbstractBlock* child);
protected:
   /*!
    * This interface makes a new block object of this block's type with no data and 
    * returns a pointer to the new block. 
    *
    * @return Pointer to the newly created block. 
    */
   virtual std::unique_ptr<AbstractBlock> makeBlank() const = 0;
   /*!
    * This interface returns the current data version for this block type. This 
    * version is saved to this block's data when written and then used when read in 
    * for backwards compatibility. 
    *
    * @return Current data version. 
    */
   virtual int version() const = 0;
   /*!
    * This interface returns the tag name for the field with the given index for this 
    * block. The tag name must be unique among all fields and is used for reading and 
    * writing this block. 
    *
    * @param index Index of the field whose tag name is returned. 
    *
    * @return Tag name for the field with the given index for this block. 
    */
   virtual QString fieldTag(int index) const = 0;
   /*!
    * This interface returns the index of the field that has the given tag name for 
    * this block. If no field is found with the given tag name this returns -1. 
    *
    * @param name Tag name of the field whose index is returned. 
    *
    * @return Index of the field with the given tag name or -1 if no field exists with 
    *         that tag name. 
    */
   virtual int fieldIndexOf(const QString& name) const = 0;
   /*!
    * This interface is called when the field with the given index for this block has 
    * been modified. 
    *
    * @param index Index of the field which has just been modified. 
    */
   virtual void fieldModified(int index) = 0;
   /*!
    * This interface quietly sets the value of the field with the given index to the 
    * new given value. This must be done quietly without calling any notify methods 
    * because the base abstract class has already done so using the field modified 
    * interface. 
    *
    * @param index Index of the field whose value is set to the new given value. 
    *
    * @param value New value that the field with the given index is set to. 
    */
   virtual void quietlySetField(int index, const QVariant& value) = 0;
   virtual bool childNameModified(AbstractBlock* child);
   virtual bool childAdded(AbstractBlock* child);
   virtual bool childRemoved(AbstractBlock* child);
   virtual bool childMoved(AbstractBlock* child);
protected:
   int dataVersion() const;
   void notifyModified();
   void notifyNameModified();
   void notifyBodyModified();
private:
   void readData(const QDomElement& element);
   QDomElement writeData(QDomDocument& document) const;
   void copyChildren(const AbstractBlock* parent);
   void copyDataFrom(const AbstractBlock* other);
   void readChild(const QDomElement& element);
   void setParent(AbstractBlock* parent, int index = -1);
   /*!
    * The name for version attributes. 
    */
   static const char* _versionTag;
   /*!
    * The tag name for data elements. 
    */
   static const char* _dataTag;
   /*!
    * The name for type attributes. 
    */
   static const char* _typeTag;
   /*!
    * The data version of the data this block is reading in or the current version if 
    * no data is being read in. 
    */
   int _version;
   /*!
    * Pointer list of this block's children. 
    */
   QList<AbstractBlock*> _children;
};






/*!
 * Build a list of this block's children that matches the given type. The returned 
 * list has no other copies. 
 *
 * @tparam T The child class type that is matched. 
 *
 * @param type The type whose matches are added to the list. 
 *
 * @return List of this block's children that is given type. 
 */
template<class T> QList<T*> AbstractBlock::makeListOfType(int type) const
{
   // Create a new list of pointers _ret_ of the given template type. Iterate through 
   // the list of this block's children. If a child matches the given type then 
   // append its pointer to _ret_. 
   QList<T*> ret;
   for (auto child : list())
   {
      if ( T* variable = child->cast<T>(type) ) ret.append(variable);
   }

   // Return _ret_. 
   return ret;
}






/*!
 * This casts this block to a specific block type. If the given type does not match 
 * this block's type then a null pointer is returned. If this object fails to cast 
 * as the requested type an exception is thrown. 
 *
 * @tparam T The class type that is cast. 
 *
 * @param toType The block type that this block is cast to. 
 *
 * @return If successful then a read only pointer of requested cast type, else a 
 *         null pointer. 
 */
template<class T> const T* AbstractBlock::cast(int toType) const
{
   // If the given type to cast does not match this block's type then return a null 
   // pointer. 
   if ( type() != toType )
   {
      return nullptr;
   }

   // Cast this block's pointer to the requested class type and return the cast 
   // pointer. If the cast fails then throw an exception. 
   if ( const T* ret = qobject_cast<const T*>(this) ) return ret;
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed casting object to required type."));
      throw e;
   }
}






/*!
 * This casts this block to a specific block type. If the given type does not match 
 * this block's type then a null pointer is returned. If this object fails to cast 
 * as the requested type an exception is thrown. 
 *
 * @tparam T The class type that is cast. 
 *
 * @param toType The block type that this block is cast to. 
 *
 * @return If successful then a pointer of requested cast type, else a null 
 *         pointer. 
 */
template<class T> T* AbstractBlock::cast(int toType)
{
   // If the given type to cast does not match this block's type then return a null 
   // pointer. 
   if ( type() != toType )
   {
      return nullptr;
   }

   // Cast this block's pointer to the requested class type and return the cast 
   // pointer. If the cast fails then throw an exception. 
   if ( T* ret = qobject_cast<T*>(this) ) return ret;
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Failed casting object to required type."));
      throw e;
   }
}



#endif
