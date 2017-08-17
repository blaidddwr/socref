#ifndef ABSTRACTBLOCKFACTORY_H
#define ABSTRACTBLOCKFACTORY_H
#include <QString>

#include "singletonfactory.h"



class AbstractBlock;
class AbstractView;
class AbstractEdit;



// @@ AbstractBlockFactory
class AbstractBlockFactory : public SingletonFactory<AbstractBlockFactory>
{
public:
   virtual int getSize() const = 0;
   virtual QString getName() const = 0;
   virtual QList<int> getBuildList(int type) const = 0;
   virtual AbstractBlock* makeBlock(int type) const = 0;
   virtual AbstractView* makeView(int type) const = 0;
   virtual AbstractEdit* makeEdit(int type) const = 0;
};



#endif
