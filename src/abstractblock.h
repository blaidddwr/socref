#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>
#include <QObject>
#include <QDomElement>
#include "global.h"



class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   virtual QString name() const = 0;
   virtual std::unique_ptr<AbstractBlock> makeCopy() const = 0;
   virtual int type() const = 0;
   virtual const AbstractBlockFactory& factory() const = 0;
   AbstractBlock* parent() const;
   int childrenSize() const;
   AbstractBlock* child(int index);
   const AbstractBlock* child(int index) const;
   int childIndex(AbstractBlock* child) const;
   void insertChild(int index, std::unique_ptr<AbstractBlock>&& child);
   std::unique_ptr<AbstractBlock> takeChild(int index);
   void removeChild(int index);
   void read(const QDomElement& parent);
   QDomElement write(QDomDocument& document) const;
protected:
   virtual void readData(const QDomElement& data) = 0;
   virtual QDomElement writeData(QDomDocument& document) const = 0;
   AbstractBlock* root();
   void copyChildren(const AbstractBlock* block);
   QList<AbstractBlock*> children() const;
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



#endif
