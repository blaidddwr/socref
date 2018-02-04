#ifndef ABSTRACTBLOCKFACTORY_H
#define ABSTRACTBLOCKFACTORY_H
#include <memory>
#include <QString>
#include <QIcon>
#include "global.h"
#include "gui.h"



class AbstractBlockFactory
{
public:
   virtual int size() const = 0;
   virtual QString name(int type) const = 0;
   virtual QString elementName(int type) const = 0;
   virtual std::unique_ptr<AbstractBlock> makeRootBlock() const = 0;
   virtual std::unique_ptr<AbstractBlock> makeBlock(int type) const = 0;
   virtual std::unique_ptr<QWidget> makeView(int type, AbstractBlock* block) const = 0;
   virtual std::unique_ptr<Gui::AbstractEdit> makeEdit(int type, AbstractBlock* block) const = 0;
   virtual int type() const = 0;
private:
   int _type;
};



#endif
