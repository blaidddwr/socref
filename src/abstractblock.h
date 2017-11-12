#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <QObject>



class AbstractBlockFactory;
class QXmlStreamReader;
class QXmlStreamWriter;



class AbstractBlock : public QObject
{
   Q_OBJECT
public:
   explicit AbstractBlock(const AbstractBlockFactory& factory, int type):
      _factory(factory),
      _type(type),
      _root(this)
      {}
   virtual QString getName() const = 0;
   virtual void readData(QXmlStreamReader& xml) = 0;
   virtual void writeData(QXmlStreamWriter& xml) const = 0;
   virtual AbstractBlock* makeCopy() const = 0;
   int getType() const { return _type; }
   const AbstractBlockFactory& getFactory() const { return _factory; }
   AbstractBlock* getParent() const { return _parent; }
   int getChildrenSize() const { return _children.size(); }
   AbstractBlock* getChild(int index);
   const AbstractBlock* getChild(int index) const;
   int getChildIndex(AbstractBlock* child) const
   { return _children.indexOf(child); }
   void insertChild(int index, AbstractBlock* takenChild);
   AbstractBlock* takeChild(int index);
   void removeChild(int index);
   void read(QXmlStreamReader& xml);
   void write(QXmlStreamWriter& xml) const;
protected:
   AbstractBlock& getRoot() { return *_root; }
   void copyChildren(const AbstractBlock* block);
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
   AbstractBlock* _root;
   QList<AbstractBlock*> _children;
};



#endif
