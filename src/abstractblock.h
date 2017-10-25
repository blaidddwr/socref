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
   explicit AbstractBlock(const AbstractBlockFactory& factory);
   //@@
   virtual QString getName() const = 0;
   //@@
   virtual void readData(QXmlStreamReader& xml) = 0;
   //@@
   virtual void writeData(QXmlStreamWriter& xml) const = 0;
   //@@
   virtual AbstractBlock* makeCopy() const = 0;
   void initialize(int type, AbstractBlock* parent = nullptr);
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
signals:
   void modified();
private slots:
   void childModified();
private:
   void setParent(AbstractBlock* parent, int index = -1);
   void readChildren(QXmlStreamReader& xml);
   void readChild(QXmlStreamReader& xml);
   const AbstractBlockFactory& _factory;
   int _type {-1};
   QList<AbstractBlock*> _children;
};



//@@
inline AbstractBlock::AbstractBlock(const AbstractBlockFactory& factory): _factory(factory) {}

//@@
inline int AbstractBlock::getType() const { return _type; }

//@@
inline const AbstractBlockFactory &AbstractBlock::getFactory() const { return _factory; }

//@@
inline AbstractBlock *AbstractBlock::getParent() const
   { return qobject_cast<AbstractBlock*>(parent()); }

//@@
inline int AbstractBlock::getChildrenSize() const { return _children.size(); }

//@@
inline int AbstractBlock::getChildIndex(AbstractBlock *child) const
   { return _children.indexOf(child); }

//@@
inline void AbstractBlock::childModified() { emit modified(); }



#endif
