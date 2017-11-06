#ifndef CPPQT_TYPE_H
#define CPPQT_TYPE_H
#include <QList>



class QXmlStreamReader;
class QXmlStreamWriter;



//@@
namespace CppQt
{
   //@@
   class Type
   {
   public:
      Type() = default;
      Type(const QString& name);
      Type(const QString& name, const QList<QString>& templateArguments);
      Type(const QList<QString>& scope, const QString& name);
      Type(const QList<QString>& scope, const QString& name
           , const QList<QString>& templateArguments);
      QString getName(const QList<QString>& scope = QList<QString>()) const;
      bool isTemplate() const;
      bool isConcrete() const;
      bool isBasic() const;
      int getTemplateSize() const;
      QString getTemplateName(int index) const;
      Type& setTemplateValues(const QList<Type>& values);
      Type& clearTemplateValues();
      Type& setPointers(const QList<bool>& pointers, bool reference);
      Type& setConstants(bool valueConstant, bool expressionConstant);
      Type& setStatic(bool isStatic);
      Type& appendNamespace(const QString& name);
      Type& prependNamespace(const QString& name);
      bool isConstantValue() const;
      int getPointerSize() const;
      bool isConstantPointer(int index) const;
      bool isReference() const;
      bool operator!=(const Type& type);
      bool operator==(const Type& type);
      friend QXmlStreamReader& operator>>(QXmlStreamReader& xml, Type& type);
      friend QXmlStreamWriter& operator<<(QXmlStreamWriter& xml, const Type& type);
   private:
      void readTemplateElement(QXmlStreamReader& xml);
      static bool readBoolean(QXmlStreamReader& xml);
      QList<QString> _scope;
      QString _name;
      QList<QString> _templateArguments;
      QList<Type> _templateValues;
      QList<bool> _pointers;
      bool _reference {false};
      bool _valueConstant {false};
      bool _expressionConstant {false};
      bool _static {false};
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
   inline bool Type::isBasic() const { return _templateValues.isEmpty() && _pointers.isEmpty()
            && !_reference && !_valueConstant && !_expressionConstant && !_static; }

   //@@
   inline int Type::getTemplateSize() const { return _templateArguments.size(); }

   //@@
   inline bool Type::isConstantValue() const { return _valueConstant; }

   //@@
   inline int Type::getPointerSize() const { return _pointers.size(); }

   //@@
   inline bool Type::isReference() const { return _reference; }

   //@@
   inline bool Type::operator!=(const Type& type) { return _scope != type._scope
            || _name != type._name || _templateArguments != type._templateArguments
            || _templateValues != type._templateValues || _pointers != type._pointers
            || _reference != type._reference || _valueConstant != type._valueConstant; }

   //@@
   inline bool Type::operator==(const Type& type) { return _scope == type._scope
            && _name == type._name && _templateArguments == type._templateArguments
            && _templateValues == type._templateValues && _pointers == type._pointers
            && _reference == type._reference && _valueConstant == type._valueConstant; }



   QXmlStreamReader& operator>>(QXmlStreamReader& xml, Type& type);
   QXmlStreamWriter& operator<<(QXmlStreamWriter& xml, const Type& type);
}



#endif
