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
         Concrete(const QString& name):
            _name(name)
            {}
         virtual bool isConcrete() const override final { return true; }
         virtual bool isTemplate() const override final { return false; }
         virtual bool isClass() const override final { return false; }
         virtual bool isEquivalent(const AbstractType* type) const override final;
         virtual const QList<AbstractType*> contains() const override final { return QList<AbstractType*>(); }
         virtual std::unique_ptr<AbstractType> makeCopy() const override final;
         virtual QString name(const QList<QString>) const override final { return _name; }
         Concrete& setName(const QString& name);
      private:
         Concrete() = default;
         QString _name;
      };
   }
}



#endif
