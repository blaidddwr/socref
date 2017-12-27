#ifndef CPPQT_ABSTRACTCONTAINER_H
#define CPPQT_ABSTRACTCONTAINER_H
#include "abstractblock.h"



namespace CppQt
{
   class AbstractType;



   class AbstractContainer : public AbstractBlock
   {
      Q_OBJECT
   public:
      friend class ContainerModel;
      AbstractContainer(const AbstractBlockFactory& factory, int type):
         AbstractBlock(factory,type)
         {}
      const QList<AbstractType*> contains();
   protected:
      virtual QString scopeName() const = 0;
   };
}



#endif
