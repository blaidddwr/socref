#ifndef CPPQT_ABSTRACTTYPE_H
#define CPPQT_ABSTRACTTYPE_H
#include <memory>
#include <QList>
#include <QDomElement>
#include "cppqt_type_modifiers.h"



namespace CppQt
{
   class AbstractType
   {
   public:
      AbstractType() = default;
      AbstractType(const QString& name):
         _name(name)
         {}
      virtual ~AbstractType() = default;
      virtual bool isConcrete() const = 0;
      virtual bool isTemplate() const = 0;
      virtual bool isClass() const = 0;
      virtual bool isEquivalent(const AbstractType* type) const = 0;
      virtual const QList<AbstractType*> contains() const = 0;
      virtual std::unique_ptr<AbstractType> makeCopy() const = 0;
      virtual QString fullName() const = 0;
      virtual AbstractType* read(const QDomElement& type) = 0;
      virtual int type() const = 0;
      QString scopedName(bool withModifiers = false) const;
      AbstractType* setDepth(int depth);
      int depth() const { return _depth; }
      const QStringList scope() const { return _scope; }
      AbstractType* setScope(const QStringList& scope);
      AbstractType* clearScope();
      AbstractType* prependScope(const QString& scope);
      QString name() const { return _name; }
      AbstractType* setName(const QString& name);
      const Type::Modifiers& modifiers() const { return _modifiers; }
      AbstractType* setModifiers(const Type::Modifiers& modifiers);
      QDomElement write(QDomDocument& document) const;
      bool operator!=(const AbstractType& type) { return _scope != type._scope || !isEquivalent(&type); }
      bool operator==(const AbstractType& type) { return _scope == type._scope && isEquivalent(&type); }
   protected:
      virtual QDomElement writeData(QDomDocument& document) const = 0;
   private:
      int _type {-1};
      int _depth {-1};
      QStringList _scope;
      QString _name;
      Type::Modifiers _modifiers;
   };
}



#endif
