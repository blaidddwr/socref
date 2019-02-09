#ifndef ABSTRACT_BLOCK_H
#define ABSTRACT_BLOCK_H
#include <QObject>
#include <QDomElement>
#include <QIcon>
#include <socutil/soc_ut_qptr.h>
#include "abstract.h"
//



namespace Abstract
{
   /*!
    * This represents a single block in this application's reference system. The block 
    * system is the core function of this application, representing blocks as a single 
    * atomic item for referencing code. The block system conforms to a parent child 
    * relationship. Virtual interfaces are provided for basic information, spawning 
    * view and abstract edit widgets. Interfaces are also provided for reading from 
    * and writing to XML elements. 
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
    * final deals with reading from and writing to XML elements used for saving 
    * projects to an XML file. Optional interfaces are also provides for dealing with 
    * child events, such as children being added or removed. 
    * 
    * This class itself handles all copying using another specialized interface for 
    * copying implementation data and another regular method for copying children 
    * blocks. This class also handles all management of children internally. 
    */
   class Block : public QObject
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
      virtual const Abstract::BlockFactory& factory() const = 0;
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
       * This interface creates and returns a block view that provides a detailed read 
       * only GUI representation of this block's data. 
       *
       * @return New GUI view that represents this block's data. 
       */
      virtual Soc::Ut::QPtr<BlockView> createView() const = 0;
      virtual Soc::Ut::QPtr<Abstract::Block> createCopy() const;
      /*!
       * This interface creates and returns an abstract edit GUI dialog that provides the 
       * ability to edit this block's data. 
       *
       * @return Abstract edit GUI dialog to edit this block's data. 
       */
      virtual Soc::Ut::QPtr<Abstract::BlockEdit> createEdit() = 0;
   public:
      const Abstract::Block* root() const;
      Abstract::Block* parent() const;
      int size() const;
      const QList<Abstract::Block*>& list() const;
      int indexOf(Block* pointer) const;
      Abstract::Block* get(int index) const;
      bool containsType(int type) const;
      bool containsType(const QList<int>& types) const;
      template<class T> QList<T*> createListOfType(int type) const;
      template<class T> const T* cast(int toType) const;
      QDomElement write(QDomDocument& document) const;
      Abstract::Block* root();
      template<class T> T* cast(int toType);
      void moveUp(int index);
      void moveDown(int index);
      void insert(int index, Soc::Ut::QPtr<Block>&& child);
      Soc::Ut::QPtr<Abstract::Block> take(int index);
      void remove(int index);
      void read(const QDomElement& element);
   signals:
      /*!
       * Signals that this block has been updated and any view attached to it must update 
       * this block's display. 
       */
      void updated();
      /*!
       * Signals that a child block of this block has been updated. The given child block 
       * could be any depth below this block. Only the root block emits this signal. 
       *
       * @param child The child block which has been updated. 
       */
      void childUpdated(Block* child);
   protected:
      /*!
       * This interface reads in the data for this block from the given XML element, 
       * overwriting any data this block currently holds. 
       *
       * @param element The XML element used to read in this blocks data. 
       */
      virtual void readData(const QDomElement& element) = 0;
      /*!
       * This interface returns a XML element containing the data for this block using 
       * the current version number. Attributes should never be used with this element 
       * because of their indeterminate nature. 
       *
       * @param document XML document to use for creating new elements. 
       *
       * @return XML element containing the data of this block. 
       */
      virtual QDomElement writeData(QDomDocument& document) const = 0;
      /*!
       * This interface creates and returns a new block object of this block's type with 
       * no data and returns a pointer to the new block. 
       *
       * @return The newly created blank block. 
       */
      virtual Soc::Ut::QPtr<Abstract::Block> createBlank() const = 0;
      /*!
       * This interface copies all data from the given block to this block, overwriting 
       * any data this block may already contain. This does not copy any children. 
       *
       * @param other The other block whose data will be copied. 
       */
      virtual void copyDataFrom(const Block* other) = 0;
      virtual bool childIsUpdated(Block* child);
      virtual bool childAdded(Block* child);
      virtual bool childRemoved(Block* child);
      virtual bool childMoved(Block* child);
   protected:
      void update();
   private:
      void copyChildren(const Block* parent);
      void readChild(const QDomElement& element);
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
       * Pointer list of this block's children. 
       */
      QList<Block*> _children;
   };






   /*!
    * creates and returns a list of this block's children that matches the given type. 
    * The returned list has no other copies. 
    *
    * @tparam T The child class type that is matched. 
    *
    * @param type The type whose matches are added to the list. 
    *
    * @return List of this block's children that is given type. 
    */
   template<class T> QList<T*> Block::createListOfType(int type) const
   {
      // Generate a pointer list of all this block's children that match the given type. 
      QList<T*> ret;
      for (auto child : list())
      {
         if ( T* variable = child->cast<T>(type) ) ret.append(variable);
      }
   
      // Return the pointer list of matched children. 
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
   template<class T> const T* Block::cast(int toType) const
   {
      // Make sure the given type matches this block's type. 
      if ( type() != toType )
      {
         return nullptr;
      }
   
      // Cast this block to the given type and make sure it works. 
      const T* ret = qobject_cast<const T*>(this);
      Q_CHECK_PTR(ret);
   
      // Return the type casted pointer. 
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
    * @return If successful then a pointer of requested cast type, else a null 
    *         pointer. 
    */
   template<class T> T* Block::cast(int toType)
   {
      // Make sure the given type matches this block's type. 
      if ( type() != toType )
      {
         return nullptr;
      }
   
      // Cast this block to the given type and make sure it works. 
      T* ret = qobject_cast<T*>(this);
      Q_CHECK_PTR(ret);
   
      // Return the type casted pointer. 
      return ret;
   }
}



#endif
