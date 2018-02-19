#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>
#include <QObject>
#include <QDomElement>
#include <exception.h>
#include "global.h"
#include "gui.h"



class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   virtual QString name() const = 0;
   virtual int type() const = 0;
   virtual const AbstractBlockFactory& factory() const = 0;
   virtual QIcon icon() const = 0;
   virtual QList<int> buildList() const = 0;
   virtual std::unique_ptr<QWidget> makeView() const = 0;
   virtual std::unique_ptr<Gui::AbstractEdit> makeEdit() = 0;
   virtual std::unique_ptr<AbstractBlock> makeCopy() const;
   AbstractBlock* root();
   const AbstractBlock* root() const;
   AbstractBlock* parent() const;
   int childrenSize() const;
   AbstractBlock* child(int index);
   AbstractBlock* child(int index) const;
   QList<AbstractBlock*> children() const;
   int childIndex(AbstractBlock* child) const;
   void insertChild(int index, std::unique_ptr<AbstractBlock>&& child);
   std::unique_ptr<AbstractBlock> takeChild(int index);
   void removeChild(int index);
   void moveChildUp(int index);
   void moveChildDown(int index);
   void read(const QDomElement& parent);
   QDomElement write(QDomDocument& document) const;
   bool hasChildOfType(int type) const;
   bool hasChildOfTypes(const QList<int>& types) const;
   template<class T> QList<T*> makeChildListOfType(int type) const;
   template<class T> T* cast(int type);
   template<class T> const T* cast(int type) const;
protected:
   virtual void readData(const QDomElement& data) = 0;
   virtual QDomElement writeData(QDomDocument& document) const = 0;
   virtual std::unique_ptr<AbstractBlock> makeBlank() const = 0;
   virtual void copyDataFrom(const AbstractBlock* object) = 0;
   void notifyOfNameChange();
signals:
   void modified();
   void nameChanged(AbstractBlock* object);
protected slots:
   virtual void childNameChanged(AbstractBlock* child);
   virtual void childAdded(AbstractBlock* child);
   virtual void childRemoved(AbstractBlock* child);
   virtual void childMoved(AbstractBlock* child);
private slots:
   void childModified();
private:
   void copyChildren(const AbstractBlock* block);
   void setBlockParent(AbstractBlock* parent, int index);
   void readChild(const QDomElement& child);
   void notifyOfNameChange(AbstractBlock* block);
   const char* _dataTag {"data"};
   const char* _typeTag {"type"};
   AbstractBlock* _parent {nullptr};
   QList<AbstractBlock*> _children;
};






template<class T> QList<T*> AbstractBlock::makeChildListOfType(int type) const
{
   QList<T*> ret;
   const QList<AbstractBlock*> list {children()};
   for (auto child : list)
   {
      if ( T* variable = child->cast<T>(type) ) ret.append(variable);
   }
   return ret;
}






template<class T> T* AbstractBlock::cast(int beType)
{
   if ( type() == beType )
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






template<class T> const T* AbstractBlock::cast(int beType) const
{
   if ( type() == beType )
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



#endif
