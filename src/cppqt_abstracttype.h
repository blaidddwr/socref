#ifndef CPPQT_ABSTRACTTYPE_H
#define CPPQT_ABSTRACTTYPE_H
#include <memory>

#include <QList>



namespace CppQt
{
   class AbstractType
   {
   public:
      virtual bool isConcrete() const = 0;
      virtual bool isTemplate() const = 0;
      virtual bool isClass() const = 0;
      virtual bool isEquivalent(const AbstractType* type) const = 0;
      virtual QList<const AbstractType*> contains() const = 0;
      virtual std::unique_ptr<AbstractType> makeCopy() const = 0;
      virtual QString name() const = 0;
      QString scopedName(const QList<QString> scope = QList<QString>()) const;
      const QList<QString> scope() const { return _scope; }
      AbstractType* clearScope();
      AbstractType* prependScope(const QString& scope);
   private:
      QList<QString> _scope;
   };
}



#endif
