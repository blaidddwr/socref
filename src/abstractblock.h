#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <QObject>



class AbstractBlockFactory;
class QXmlStreamReader;
class QXmlStreamWriter;



//@@
class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   explicit AbstractBlock(const AbstractBlockFactory& factory, int type);
   //@@
   virtual QString getName() const = 0;
   //@@
   virtual void readData(QXmlStreamReader& xml) = 0;
   //@@
   virtual void writeData(QXmlStreamWriter& xml) const = 0;
   //@@
   virtual AbstractBlock* makeCopy() const = 0;
   int getType() const;
   const AbstractBlockFactory& getFactory() const;
   AbstractBlock* getParent() const;
   int getChildrenSize() const;
   AbstractBlock* getChild(int index) const;
   int getChildIndex(AbstractBlock* child) const;
   void insertChild(int index, AbstractBlock* o_child);
   AbstractBlock* takeChild(int index);
   void removeChild(int index);
   void read(QXmlStreamReader& xml);
   void write(QXmlStreamWriter& xml) const;
protected:
   void notifyOfNameChange();
signals:
   //@@
   void modified();
   //@@
   void nameChanged(AbstractBlock* object);
private slots:
   void childModified();
private:
   void setBlockParent(AbstractBlock* parent, int index);
   void readChild(QXmlStreamReader& xml);
   void notifyOfNameChange(AbstractBlock* block);
   const AbstractBlockFactory& _factory;
   int _type {-1};
   AbstractBlock* _parent {nullptr};
   QList<AbstractBlock*> _children;
};



//@@
inline AbstractBlock::AbstractBlock(const AbstractBlockFactory& factory, int type):
   _factory(factory), _type(type) {}

//@@
inline int AbstractBlock::getType() const { return _type; }

//@@
inline const AbstractBlockFactory &AbstractBlock::getFactory() const { return _factory; }

//@@
inline AbstractBlock *AbstractBlock::getParent() const { return _parent; }

//@@
inline int AbstractBlock::getChildrenSize() const { return _children.size(); }

//@@
inline int AbstractBlock::getChildIndex(AbstractBlock *child) const
{ return _children.indexOf(child); }

//@@
inline void AbstractBlock::notifyOfNameChange() { notifyOfNameChange(nullptr); }

//@@
inline void AbstractBlock::childModified() { emit modified(); }



#endif
