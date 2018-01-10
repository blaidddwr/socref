#ifndef CPPQT_ABSTRACTCONTAINER_H
#define CPPQT_ABSTRACTCONTAINER_H
#include "abstractblock.h"
#include "cppqt_classes.h"



namespace CppQt
{
   class AbstractContainer : public AbstractBlock
   {
      Q_OBJECT
   public:
      const QList<AbstractType*> contains();
      bool exists(AbstractType* type) { return exists(contains(),{type}); }
   protected:
      bool exists(const QList<AbstractType*>& allTypes, const QList<AbstractType*>& types);
      virtual QString scopeName() const = 0;
   };
}



#endif
