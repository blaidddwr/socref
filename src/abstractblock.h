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
 */
class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   /*!
    */
   virtual int type() const = 0;
   /*!
    */
   virtual const AbstractBlockFactory& factory() const = 0;
   /*!
    */
   virtual QString name() const = 0;
   /*!
    */
   virtual QIcon icon() const = 0;
   /*!
    */
   virtual QList<int> buildList() const = 0;
   /*!
    */
   virtual std::unique_ptr<QWidget> makeView() const = 0;
   /*!
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
    */
   void modified();
   /*!
    *
    * @param object  
    */
   void nameChanged(AbstractBlock* object);
protected slots:
   virtual void childNameChanged(AbstractBlock* child);
   virtual void childAdded(AbstractBlock* child);
   virtual void childRemoved(AbstractBlock* child);
   virtual void childMoved(AbstractBlock* child);
protected:
   /*!
    *
    * @param element  
    */
   virtual void readData(const QDomElement& element) = 0;
   /*!
    *
    * @param document  
    */
   virtual QDomElement writeData(QDomDocument& document) const = 0;
   /*!
    */
   virtual std::unique_ptr<AbstractBlock> makeBlank() const = 0;
   /*!
    *
    * @param other  
    */
   virtual void copyDataFrom(const AbstractBlock* other) = 0;
   void notifyOfNameChange();
private slots:
   void childModified();
private:
   void copyChildren(const AbstractBlock* parent);
   void setParent(AbstractBlock* parent, int index = -1);
   void readChild(const QDomElement& element);
   void notifyOfNameChange(AbstractBlock* changed);
   /*!
    */
   static const char* _dataTag;
   /*!
    */
   static const char* _typeTag;
   /*!
    */
   QList<AbstractBlock*> _children;
};






/*!
 *
 * @tparam T  
 *
 * @param type  
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
 *
 * @tparam T  
 *
 * @param toType  
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
 *
 * @tparam T  
 *
 * @param toType  
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
