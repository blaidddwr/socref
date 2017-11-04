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
      Type(const QString& name, const QList<QString>& templateArguments);
      Type(const QList<QString>& scope, const QString& name);
      Type(const QList<QString>& scope, const QString& name
           , const QList<QString>& templateArguments);
      QString getName(const QList<QString>& scope) const;
      bool isTemplate() const;
      bool isConcrete() const;
      int getTemplateSize() const;
      QString getTemplateName(int index) const;
      Type& setTemplateValues(const QList<Type>& values);
      Type& clearTemplateValues();
      Type& setPointer(const QString& pointer);
      Type& setConstants(bool constantValue, bool constantPointer);
      Type& appendNamespace(const QString& name);
      Type& prependNamespace(const QString& name);
      bool isConstantValue() const;
      bool isConstantPointer() const;
      bool operator!=(const Type& type);
      bool operator==(const Type& type);
   private:
      QList<QString> _scope;
      QString _name;
      QList<QString> _templateArguments;
      QList<Type> _templateValues;
      QString _pointer;
      bool _constantValue {false};
      bool _constantPointer {false};
   };



   //@@
   inline Type::Type(const QString& name): _name(name) {}

   //@@
   inline Type::Type(const QString& name, const QList<QString>& templateArguments): _name(name),
      _templateArguments(templateArguments) {}

   //@@
   inline Type::Type(const QList<QString>& scope, const QString& name): _scope(scope), _name(name)
      {}

   //@@
   inline Type::Type(const QList<QString>& scope, const QString& name
                     , const QList<QString>& templateArguments): _scope(scope), _name(name),
      _templateArguments(templateArguments) {}

   //@@
   inline bool Type::isTemplate() const { return _templateArguments.isEmpty(); }

   //@@
   inline bool Type::isConcrete() const
   { return _templateArguments.size() == _templateValues.size(); }

   //@@
   inline int Type::getTemplateSize() const { return _templateArguments.size(); }

   //@@
   inline QString Type::getTemplateName(int index) const { return _templateArguments.at(index); }

   //@@
   inline bool Type::isConstantValue() const { return _constantValue; }

   //@@
   inline bool Type::isConstantPointer() const { return _constantPointer; }

   //@@
   inline bool Type::operator!=(const Type& type) { return _scope != type._scope
            || _name != type._name || _templateArguments != type._templateArguments
            || _templateValues != type._templateValues || _pointer != type._pointer
            || _constantValue != type._constantValue || _constantPointer != type._constantPointer; }

   //@@
   inline bool Type::operator==(const Type& type) { return _scope == type._scope
            && _name == type._name && _templateArguments == type._templateArguments
            && _templateValues == type._templateValues && _pointer == type._pointer
            && _constantValue == type._constantValue && _constantPointer == type._constantPointer; }
}



#endif
