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
         Concrete() = default;
         Concrete(const QString& name):
            AbstractType(name)
            {}
         virtual bool isConcrete() const override final { return true; }
         virtual bool isTemplate() const override final { return false; }
         virtual bool isClass() const override final { return false; }
         virtual bool isEquivalent(const AbstractType* type) const override final;
         virtual const QList<AbstractType*> contains() const override final { return QList<AbstractType*>(); }
         virtual std::unique_ptr<AbstractType> makeCopy() const override final { return std::unique_ptr<Concrete>(new Concrete(name())); }
         virtual QString fullName(const QList<QString>) const override final { return name(); }
         virtual AbstractType* read(QXmlStreamReader& xml) override final;
         virtual int type() const override final;
      private:
         virtual void writeData(QXmlStreamWriter& xml) const override final;
      };
   }
}



#endif
