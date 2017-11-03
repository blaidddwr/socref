#ifndef CPPQT_TYPE_H
#define CPPQT_TYPE_H
#include <QList>



//@@
namespace CppQt
{
   //@@
   class Type
   {
   public:
      Type(const QString& name);
      Type(const QString& name, const QList<Type>& templateArguments);
      Type(const QList<QString>& scope, const QString& name);
      Type(const QList<QString>& scope, const QString& name, const QList<Type>& templateArguments);
      Type toType(const QString& pointer, bool constantValue, bool constantPointer) const;
      QString getName(const QList<QString>& scope) const;
      bool isTemplate() const;
      bool operator!=(const Type& type);
      bool operator==(const Type& type);
   private:
      QList<QString> _scope;
      QString _name;
      QList<Type> _templateArguments;
      QString _pointer;
      bool _constantValue {false};
      bool _constantPointer {false};
   };



   //@@
   inline Type::Type(const QString& name): _name(name) {}

   //@@
   inline Type::Type(const QString& name, const QList<Type>& templateArguments): _name(name),
      _templateArguments(templateArguments) {}

   //@@
   inline Type::Type(const QList<QString>& scope, const QString& name): _scope(scope), _name(name)
      {}

   //@@
   inline Type::Type(const QList<QString>& scope, const QString& name
                     , const QList<Type>& templateArguments): _scope(scope), _name(name),
      _templateArguments(templateArguments) {}

   //@@
   inline bool Type::isTemplate() const { return _templateArguments.isEmpty(); }

   //@@
   inline bool Type::operator!=(const Type& type) { return _scope != type._scope
            || _name != type._name || _templateArguments != type._templateArguments
            || _pointer != type._pointer || _constantValue != type._constantValue
            || _constantPointer != type._constantPointer; }

   //@@
   inline bool Type::operator==(const Type& type) { return _scope == type._scope
            && _name == type._name && _templateArguments == type._templateArguments
            && _pointer == type._pointer && _constantValue == type._constantValue
            && _constantPointer == type._constantPointer; }
}



#endif
