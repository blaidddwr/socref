#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QRegExp>

#include "cppqt_type.h"
#include "exception.h"
#include "xmlelementparser.h"



using namespace CppQt;






QString Type::fullName(const QList<QString>& scope) const
{
   QString ret;
   appendPrefix(&ret);
   appendNamespace(&ret,scope);
   ret.append(_name);
   appendTemplate(&ret,scope);
   appendPointers(&ret);
   return ret;
}






bool CppQt::Type::isConcrete() const
{
   bool ret {false};
   if ( _templateNames.size() == _templateValues.size() )
   {
      ret = true;
      for (const auto& value : _templateValues)
      {
         ret &= value.isConcrete();
      }
   }
   return ret;
}






Type& Type::setTemplateNames(const QList<QString>& names)
{
   _templateNames = names;
   _templateValues.clear();
   return *this;
}






Type& Type::setTemplateValues(const QList<Type>& values)
{
   if ( values.size() != _templateNames.size() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set template values from list of size %1 when arguments list"
                               " is size %2.").arg(values.size()).arg(_templateNames.size()));
      throw e;
   }
   _templateValues = values;
   return *this;
}






Type& Type::clearTemplateValues()
{
   _templateValues.clear();
   return *this;
}






Type& Type::setPointers(const QList<bool>& pointers, bool reference)
{
   _pointers = pointers;
   _reference = reference;
   return *this;
}






Type& Type::setConstants(bool valueConstant, bool expressionConstant)
{
   _valueConstant = valueConstant;
   _expressionConstant = expressionConstant;
   return *this;
}






Type& Type::setStatic(bool isStatic)
{
   _static = isStatic;
   return *this;
}






Type& Type::appendNamespace(const QString& name)
{
   _scope.append(name);
   return *this;
}






Type& Type::prependNamespace(const QString& name)
{
   _scope.prepend(name);
   return *this;
}






bool Type::isConstantPointer(int index) const
{
   if ( index < 0 || index >= _pointers.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Pointer index %1 is out of range for %2 size list.").arg(index)
                   .arg(_pointers.size()));
      throw e;
   }
   return _pointers.at(index);
}






void Type::appendPrefix(QString* text) const
{
   if ( _expressionConstant )
   {
      text->append("constexpr ");
   }
   if ( _static )
   {
      text->append("static ");
   }
   if ( _valueConstant )
   {
      text->append("const ");
   }
}






void Type::appendNamespace(QString* text, const QList<QString>& scope) const
{
   int contextCount {0};
   int count {0};
   auto contextName {scope.cbegin()};
   for (const auto& name : _scope)
   {
      if ( count == contextCount && name == *contextName )
      {
         ++contextName;
         ++contextCount;
      }
      else
      {
         text->append(name).append("::");
      }
      ++count;
   }
}






void Type::appendTemplate(QString* text, const QList<QString>& scope) const
{
   if ( !_templateValues.isEmpty() )
   {
      text->append("<");
      for (int i = 0; i < (_templateValues.size() - 1) ;++i)
      {
         text->append(_templateValues.at(i).fullName(scope)).append(",");
      }
      text->append(_templateValues.constLast().fullName(scope)).append(">");
   }
   else if ( !_templateNames.isEmpty() )
   {
      text->append("<");
      for (int i = 0; i < (_templateNames.size() - 1) ;++i)
      {
         text->append(_templateNames.at(i)).append(",");
      }
      text->append(_templateNames.constLast()).append(">");
   }
}






void Type::appendPointers(QString* text) const
{
   for (const auto& pointer : _pointers)
   {
      text->append("*");
      if ( pointer )
      {
         text->append("const");
      }
   }
   if ( _reference )
   {
      text->append("&");
   }
}






void Type::readTemplateElement(QXmlStreamReader& xml)
{
   enum
   {
      Argument = 0
      ,Value = 1
   };
   XMLElementParser parser(xml);
   parser.addKeyword("arg",false,true).addKeyword("val",false,true);
   int element;
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      switch (element)
      {
      case Argument:
         _templateNames.append(xml.readElementText());
         break;
      case Value:
         {
            Type child;
            xml >> child;
            _templateValues.append(child);
            break;
         }
      }
   }
   if ( _templateValues.size() > 0 && _templateValues.size() != _templateNames.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Template argument size of %1 and value size of %2 is incorrect.")
                   .arg(_templateNames.size()).arg(_templateValues.size()));
      throw e;
   }
}






void Type::writeTemplateElement(QXmlStreamWriter& xml) const
{
   auto templateName {_templateNames.cbegin()};
   auto templateValue {_templateValues.cbegin()};
   while ( templateName != _templateNames.cend() )
   {
      xml.writeTextElement("arg",*templateName);
      if ( templateValue != _templateValues.cend() )
      {
         xml.writeStartElement("val");
         xml << *templateValue;
         xml.writeEndElement();
         ++templateValue;
      }
      ++templateName;
   }
}






bool Type::readBoolean(QXmlStreamReader& xml)
{
   bool ok;
   bool ret {xml.readElementText().toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Failed reading boolean element."));
      throw e;
   }
   return ret;
}






namespace CppQt
{
   QXmlStreamReader& operator>>(QXmlStreamReader& xml, Type& type)
   {
      enum
      {
         Name = 0
         ,Namespace
         ,Template
         ,Pointer
         ,Reference
         ,Constant
         ,ConstantExpression
         ,Static
      };
      type._scope.clear();
      type._templateNames.clear();
      type._templateValues.clear();
      type._pointers.clear();
      XMLElementParser parser(xml);
      parser.addKeyword("name",true).addKeyword("namespace",false,true)
            .addKeyword("template",true,true).addKeyword("pointer",false,true)
            .addKeyword("ref",true).addKeyword("const",true).addKeyword("constexpr",true)
            .addKeyword("static",true);
      int element;
      while ( ( element = parser() ) != XMLElementParser::End )
      {
         switch (element)
         {
         case Name:
            type._name = xml.readElementText();
            break;
         case Namespace:
            type._scope.append(xml.readElementText());
            break;
         case Template:
            type.readTemplateElement(xml);
            break;
         case Pointer:
            type._pointers.append(Type::readBoolean(xml));
            break;
         case Reference:
            type._reference = Type::readBoolean(xml);
            break;
         case Constant:
            type._valueConstant = Type::readBoolean(xml);
            break;
         case ConstantExpression:
            type._expressionConstant = Type::readBoolean(xml);
            break;
         case Static:
            type._static = Type::readBoolean(xml);
            break;
         }
      }
      if ( !parser.allRead() )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Failed reading in all required elements."));
         throw e;
      }
      return xml;
   }






   QXmlStreamWriter& operator<<(QXmlStreamWriter& xml, const Type& type)
   {
      xml.writeTextElement("name",type._name);
      for (const auto& name : type._scope)
      {
         xml.writeTextElement("namespace",name);
      }
      xml.writeStartElement("template");
      type.writeTemplateElement(xml);
      xml.writeEndElement();
      for (const auto& pointer : type._pointers)
      {
         xml.writeTextElement("pointer",QString::number(pointer));
      }
      xml.writeTextElement("const",QString::number(type._valueConstant));
      xml.writeTextElement("constexpr",QString::number(type._expressionConstant));
      xml.writeTextElement("static",QString::number(type._static));
      xml.writeTextElement("ref",QString::number(type._reference));
      return xml;
   }
}
