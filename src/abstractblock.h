#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>
#include <QObject>
#include <QDomElement>
#include "global.h"
#include "gui.h"



class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   virtual QString name() const = 0;
   virtual std::unique_ptr<AbstractBlock> makeCopy() const = 0;
   virtual int type() const = 0;
   virtual const AbstractBlockFactory& factory() const = 0;
   virtual QIcon icon() const = 0;
   virtual QList<int> buildList() const = 0;
   virtual std::unique_ptr<QWidget> makeView() const = 0;
   virtual std::unique_ptr<Gui::AbstractEdit> makeEdit() = 0;
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
   void read(const QDomElement& parent);
   QDomElement write(QDomDocument& document) const;
   bool hasChildOfType(int type) const;
   bool hasChildOfTypes(const QList<int>& types) const;
   template<class T> QList<T*> makeChildListOfType(int type) const;
protected:
   virtual void readData(const QDomElement& data) = 0;
   virtual QDomElement writeData(QDomDocument& document) const = 0;
   void copyChildren(const AbstractBlock* block);
   void notifyOfNameChange();
signals:
   void modified();
   void nameChanged(AbstractBlock* object);
protected slots:
   virtual void childNameChanged(AbstractBlock* child);
   virtual void childAdded(AbstractBlock* child);
   virtual void childRemoved(AbstractBlock* child);
private slots:
   void childModified();
private:
   void setBlockParent(AbstractBlock* parent, int index);
   void readChild(const QDomElement& child);
   void notifyOfNameChange(AbstractBlock* block);
   static const char* _dataTag;
   static const char* _typeTag;
   AbstractBlock* _parent {nullptr};
   QList<AbstractBlock*> _children;
};






template<class T> QList<T*> AbstractBlock::makeChildListOfType(int type) const
{
   QList<T*> ret;
   const QList<AbstractBlock*> list {children()};
   for (auto child : list)
   {
      if ( child->type() == type )
      {
         if ( T* variable = qobject_cast<T*>(child) ) ret.append(variable);
      }
   }
   return ret;
}



#endif
