#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <memory>

#include <QObject>

#include "abstractblockfactory.h"



class QXmlStreamReader;
class QXmlStreamWriter;
class AbstractBlockFactory;



class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   explicit AbstractBlock(const AbstractBlockFactory& factory, int type):
      _factory(factory),
      _type(type)
      {}
   virtual QString name() const = 0;
   virtual std::unique_ptr<AbstractBlock> makeCopy() const = 0;
   int type() const { return _type; }
   const AbstractBlockFactory& factory() const { return _factory; }
   AbstractBlock* parent() const { return _parent; }
   int childrenSize() const { return _children.size(); }
   AbstractBlock* child(int index);
   const AbstractBlock* child(int index) const;
   int childIndex(AbstractBlock* child) const
   { return _children.indexOf(child); }
   AbstractBlock* insertChild(int index, std::unique_ptr<AbstractBlock> child);
   std::unique_ptr<AbstractBlock> takeChild(int index);
   AbstractBlock* removeChild(int index);
   AbstractBlock* read(QXmlStreamReader& xml);
   const AbstractBlock* write(QXmlStreamWriter& xml) const;
protected:
   virtual AbstractBlock* readData(QXmlStreamReader& xml) = 0;
   virtual const AbstractBlock* writeData(QXmlStreamWriter& xml) const = 0;
   AbstractBlock& root();
   AbstractBlock* copyChildren(const AbstractBlock* block);
   void notifyOfNameChange() { notifyOfNameChange(nullptr); }
signals:
   void modified();
   void nameChanged(AbstractBlock* object);
private slots:
   void childModified() { emit modified(); }
private:
   void setBlockParent(AbstractBlock* parent, int index);
   void readChild(QXmlStreamReader& xml);
   void notifyOfNameChange(AbstractBlock* block);
   const AbstractBlockFactory& _factory;
   int _type {-1};
   AbstractBlock* _parent {nullptr};
   QList<AbstractBlock*> _children;
};



#endif
