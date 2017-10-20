#ifndef ABSTRACTBLOCKFACTORY_H
#define ABSTRACTBLOCKFACTORY_H
#include <QString>

#include "singletonfactory.h"



class AbstractBlock;

namespace Gui {

class AbstractView;
class AbstractEdit;

} // namespace Gui



//@@
class AbstractBlockFactory : public SingletonFactory<AbstractBlockFactory>
{
public:
   //@@
   virtual int getSize() const = 0;
   //@@
   virtual int getSlotSize(int type) const = 0;
   //@@
   virtual QString getName(int type) const = 0;
   //@@
   virtual QString getSlotName(int type, int slot) const = 0;
   //@@
   virtual const QList<int> getBuildList(int type, int slot) const = 0;
   //@@
   virtual AbstractBlock* makeRootBlock() const = 0;
   //@@
   virtual AbstractBlock* makeBlock(int type) const = 0;
   //@@
   virtual Gui::AbstractView* makeView(int type) const = 0;
   //@@
   virtual Gui::AbstractEdit* makeEdit(int type) const = 0;
};



#endif
