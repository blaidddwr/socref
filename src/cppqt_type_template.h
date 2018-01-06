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
         Template() = default;
         Template(const QString& name, const QList<QString>& variants = QList<QString>());
         virtual ~Template() { qDeleteAll(_values); }
         virtual bool isConcrete() const override final;
         virtual bool isTemplate() const override final { return true; }
         virtual bool isClass() const override { return false; }
         virtual bool isEquivalent(const AbstractType* type) const override final;
         virtual const QList<AbstractType*> contains() const override final { return _values; }
         virtual std::unique_ptr<AbstractType> makeCopy() const override final { return std::unique_ptr<Template>(new Template(name(),_variants)); }
         virtual QString fullName(const QList<QString>& scope) const override final;
         virtual AbstractType* read(QXmlStreamReader& xml) override final;
         virtual int type() const override final;
         const QList<QString> variants() const { return _variants; }
         Template& setVariants(const QList<QString>& variants);
         Template& setValue(int index, std::unique_ptr<AbstractType>&& type);
         Template& clearValues();
      private:
         virtual void writeData(QXmlStreamWriter& xml) const override final;
         QList<QString> _variants;
         QList<AbstractType*> _values;
      };
   }
}



#endif
