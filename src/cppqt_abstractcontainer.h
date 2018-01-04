#ifndef CPPQT_ABSTRACTCONTAINER_H
#define CPPQT_ABSTRACTCONTAINER_H
#include "abstractblock.h"
#include "cppqt_abstracttype.h"



namespace CppQt
{
   class AbstractContainer : public AbstractBlock
   {
      Q_OBJECT
   public:
      friend class ContainerModel;
      explicit AbstractContainer(const AbstractBlockFactory& factory, int type):
         AbstractBlock(factory,type)
         {}
      const QList<AbstractType*> contains();
   protected:
      virtual QString scopeName() const = 0;
   };
}



#endif
