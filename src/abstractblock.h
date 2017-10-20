#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H
#include <QObject>



class QXmlStreamReader;
class QXmlStreamWriter;



//@@
class AbstractBlock : public QObject
{
   Q_OBJECT
public:
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
   AbstractBlock* getParent() const;
   int getChildrenSize() const;
   AbstractBlock* getChild(int index) const;
   int getChildIndex(AbstractBlock* child) const;
   void insertChild(int index, AbstractBlock* child = nullptr);
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
   int _type {-1};
   QList<AbstractBlock*> _children;
};



//@@
inline int AbstractBlock::getType() const { return _type; }

//@@
inline AbstractBlock *AbstractBlock::getParent() const
   { return qobject_cast<AbstractBlock*>(parent()); }

//@@
inline int AbstractBlock::getChildrenSize() const { return _children.size(); }

//@@
inline void AbstractBlock::childModified() { emit modified(); }



#endif
