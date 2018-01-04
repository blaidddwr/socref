#ifndef CPPQT_ABSTRACTTYPE_H
#define CPPQT_ABSTRACTTYPE_H
#include <memory>

#include <QList>



namespace CppQt
{
   class AbstractType
   {
   public:
      virtual ~AbstractType() = default;
      virtual bool isConcrete() const = 0;
      virtual bool isTemplate() const = 0;
      virtual bool isClass() const = 0;
      virtual bool isEquivalent(const AbstractType* type) const = 0;
      virtual const QList<AbstractType*> contains() const = 0;
      virtual std::unique_ptr<AbstractType> makeCopy() const = 0;
      virtual QString name(const QList<QString> scope = QList<QString>()) const = 0;
      QString scopedName(const QList<QString> scope = QList<QString>()) const;
      const QList<QString> scope() const { return _scope; }
      AbstractType* clearScope();
      AbstractType* prependScope(const QString& scope);
      bool operator!=(const AbstractType& type) { return _scope != type._scope || !isEquivalent(&type); }
      bool operator==(const AbstractType& type) { return _scope == type._scope && isEquivalent(&type); }
   private:
      QList<QString> _scope;
   };
}



#endif
