#ifndef CPPQT_TYPE_CONCRETE_H
#define CPPQT_TYPE_CONCRETE_H
#include "cppqt_abstracttype.h"



namespace CppQt
{
   namespace Type
   {
      class Concrete : public AbstractType
      {
      public:
         virtual bool isConcrete() const override final { return true; }
         virtual bool isTemplate() const override final { return false; }
         virtual bool isClass() const override final { return false; }
         virtual bool isEquivalent(const AbstractType* type) const override final;
         virtual QList<const AbstractType*> contains() const override final { return QList<const AbstractType*>(); }
         virtual std::unique_ptr<AbstractType> makeCopy() const override final;
         QString name() const { return _name; }
         Concrete& setName(const QString& name);
      private:
         virtual QString rawName() const override final { return _name; }
         QString _name {"unnamed_type"};
      };
   }
}



#endif
