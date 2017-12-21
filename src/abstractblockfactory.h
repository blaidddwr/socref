#ifndef ABSTRACTBLOCKFACTORY_H
#define ABSTRACTBLOCKFACTORY_H
#include <memory>
#include <QString>
#include <QIcon>



class AbstractBlock;

namespace Gui
{
   class AbstractEdit;
}



class AbstractBlockFactory
{
public:
   AbstractBlockFactory(int type):
      _type(type)
      {}
   virtual int size() const = 0;
   virtual QString name(int type) const = 0;
   virtual QString elementName(int type) const = 0;
   virtual QIcon icon(int type) const = 0;
   virtual const QList<int> buildList(int type) const = 0;
   virtual std::unique_ptr<AbstractBlock> makeRootBlock() const = 0;
   virtual std::unique_ptr<AbstractBlock> makeBlock(int type) const = 0;
   virtual std::unique_ptr<QWidget> makeView(int type, AbstractBlock* block) const = 0;
   virtual std::unique_ptr<Gui::AbstractEdit> makeEdit(int type, AbstractBlock* block) const = 0;
   int type() const { return _type; }
private:
   int _type;
};



#endif
