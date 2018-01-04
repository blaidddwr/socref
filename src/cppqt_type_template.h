#ifndef CPPQT_TYPE_TEMPLATE_H
#define CPPQT_TYPE_TEMPLATE_H
#include <memory>

#include "cppqt_abstracttype.h"



namespace CppQt
{
   namespace Type
   {
      class Template : public AbstractType
      {
      public:
         Template(const QString& name, QList<QString> variants = QList<QString>());
         virtual ~Template() { qDeleteAll(_values); }
         virtual bool isConcrete() const override final;
         virtual bool isTemplate() const override final { return true; }
         virtual bool isClass() const override { return false; }
         virtual bool isEquivalent(const AbstractType* type) const override final;
         virtual const QList<AbstractType*> contains() const override final { return _values; }
         virtual std::unique_ptr<AbstractType> makeCopy() const override final { return std::unique_ptr<Template>(new Template(_name,_variants)); }
         virtual QString name(const QList<QString> scope) const override final;
         const QList<QString> variants() const { return _variants; }
         Template& setVariants(const QList<QString>& variants);
         Template& setValue(int index, std::unique_ptr<AbstractType>&& type);
         Template& clearValues();
      private:
         Template() = default;
         QString _name;
         QList<QString> _variants;
         QList<AbstractType*> _values;
      };
   }
}



#endif
