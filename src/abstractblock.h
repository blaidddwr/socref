#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>
#include <QObject>
#include <QDomElement>
#include "classes.h"



class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   virtual QString name() const = 0;
   virtual std::unique_ptr<AbstractBlock> makeCopy() const = 0;
   virtual int type() const = 0;
   virtual const AbstractBlockFactory& factory() const = 0;
   AbstractBlock* parent() const { return _parent; }
   int childrenSize() const { return _children.size(); }
   AbstractBlock* child(int index);
   const AbstractBlock* child(int index) const;
   int childIndex(AbstractBlock* child) const
   { return _children.indexOf(child); }
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
   void notifyOfNameChange();
signals:
   void modified();
   void nameChanged(AbstractBlock* object);
private slots:
   void childModified() { emit modified(); }
private:
   void setBlockParent(AbstractBlock* parent, int index);
   void readChild(const QDomElement& child);
   void notifyOfNameChange(AbstractBlock* block);
   AbstractBlock* _parent {nullptr};
   QList<AbstractBlock*> _children;
};



#endif
