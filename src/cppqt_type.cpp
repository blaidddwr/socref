#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QRegExp>

#include "cppqt_type.h"
#include "exception.h"
#include "xmlelementparser.h"



using namespace CppQt;






//@@
QString Type::getName(const QList<QString>& scope) const
{
   // create return string
   QString ret;

   // if type is expression constant add it to string
   if ( _expressionConstant )
   {
      ret.append("constexpr ");
   }

   // if type is static add it to string
   if ( _static )
   {
      ret.append("static ");
   }

   // if type is constant value add it to string
   if ( _valueConstant )
   {
      ret.append("const ");
   }

   // keep track of level of namespaces using counters for both scopes
   int contextCount {0};
   int count {0};

   // iterate through type's scope while keeping a pointer to contextual scope
   auto contextName {scope.cbegin()};
   for (const auto& name : _scope)
   {
      // check if both scopes are equal and at the same depth
      if ( count == contextCount && name == *contextName )
      {
         // increment contextual scope and counter
         ++contextName;
         ++contextCount;
      }

      // else they do not match so add namespace to type name
      else
      {
         ret.append(name).append("::");
      }

      // either way increment count
      ++count;
   }

   // append basic type name
   ret.append(_name);

   // check if template values are set
   if ( !_templateValues.isEmpty() )
   {
      // iterate through all template values and append it to type name string
      ret.append("<");
      for (int i = 0; i < (_templateValues.size() - 1) ;++i)
      {
         ret.append(_templateValues.at(i).getName(scope)).append(",");
      }
      ret.append(_templateValues.constLast().getName(scope)).append(">");
   }

   // else check if type has any template arguments
   else if ( !_templateArguments.isEmpty() )
   {
      // iterate through all template arguments and append it to type name string
      ret.append("<");
      for (int i = 0; i < (_templateArguments.size() - 1) ;++i)
      {
         ret.append(_templateArguments.at(i)).append(",");
      }
      ret.append(_templateArguments.constLast()).append(">");
   }

   // append all pointers to end
   for (const auto& pointer : _pointers)
   {
      ret.append("*");
      if ( pointer )
      {
         ret.append("const");
      }
   }

   // if there is reference add to end
   if ( _reference )
   {
      ret.append("&");
   }

   // return constructed type name
   return ret;
}






//@@
bool CppQt::Type::isConcrete() const
{
   // initialize return value and check if template argument and value sizes are the same
   bool ret {false};
   if ( _templateArguments.size() == _templateValues.size() )
   {
      // set return value to true and make sure all template values are also concrete
      ret = true;
      for (const auto& value : _templateValues)
      {
         ret &= value.isConcrete();
      }
   }

   // return value
   return ret;
}






//@@
QString Type::getTemplateName(int index) const
{
   // make sure index is within range
   if ( index < 0 || index >= _templateArguments.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Template index %1 is out of range for %2 size list.").arg(index)
                   .arg(_templateArguments.size()));
      throw e;
   }

   // return argument at given index
   return _templateArguments.at(index);
}






//@@
Type& Type::setTemplateValues(const QList<Type>& values)
{
   // make sure list size of values is the same as size of arguments
   if ( values.size() != _templateArguments.size() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set template values from list of size %1 when arguments list"
                               " is size %2.").arg(values.size()).arg(_templateArguments.size()));
      throw e;
   }

   // set template values and return referene to this
   _templateValues = values;
   return *this;
}






//@@
Type& Type::clearTemplateValues()
{
   // clear template values and return reference to this
   _templateValues.clear();
   return *this;
}






//@@
Type& Type::setPointers(const QList<bool>& pointers, bool reference)
{
   // set pointers and reference
   _pointers = pointers;
   _reference = reference;

   // return reference to this
   return *this;
}






//@@
Type& Type::setConstants(bool valueConstant, bool expressionConstant)
{
   // set value and expression constness
   _valueConstant = valueConstant;
   _expressionConstant = expressionConstant;

   // return reference to this
   return *this;
}






//@@
Type& Type::setStatic(bool isStatic)
{
   // set static and return reference ot this
   _static = isStatic;
   return *this;
}









//@@
Type& Type::appendNamespace(const QString& name)
{
   // append new namespace and return reference to this
   _scope.append(name);
   return *this;
}






//@@
Type& Type::prependNamespace(const QString& name)
{
   // prepend new namespace and return reference to this
   _scope.prepend(name);
   return *this;
}






//@@
bool Type::isConstantPointer(int index) const
{
   // make sure index is within range
   if ( index < 0 || index >= _pointers.size() )
   {
      Exception::OutOfRange e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Pointer index %1 is out of range for %2 size list.").arg(index)
                   .arg(_templateArguments.size()));
      throw e;
   }

   // return constant trueness for pointer
   return _pointers.at(index);
}






//@@
void Type::readTemplateElement(QXmlStreamReader& xml)
{
   // enumeration of elements to read
   enum
   {
      Argument = 0
      ,Value = 1
   };

   // initialize xml element parser
   XMLElementParser parser(xml);
   parser.addKeyword("arg",false,true).addKeyword("val",false,true);
   int element;

   // parse xml until end of root element is reached
   while ( ( element = parser() ) != XMLElementParser::End )
   {
      // determine which element is found and set data
      switch (element)
      {
      case Argument:
         _templateArguments.append(xml.readElementText());
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

   // make sure template lists are correct
   if ( _templateValues.size() > 0 && _templateValues.size() != _templateArguments.size() )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Template argument size of %1 and value size of %2 is incorrect.")
                   .arg(_templateArguments.size()).arg(_templateValues.size()));
      throw e;
   }
}






//@@
bool Type::readBoolean(QXmlStreamReader& xml)
{
   // read in boolean value and make sure it worked
   bool ok;
   bool ret {xml.readElementText().toInt(&ok)};
   if ( !ok )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Failed reading boolean element."));
      throw e;
   }

   // return boolean value
   return ret;
}






namespace CppQt
{
   //@@
   QXmlStreamReader& operator>>(QXmlStreamReader& xml, Type& type)
   {
      // enumeration of elements to read
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

      // clear all lists
      type._scope.clear();
      type._templateArguments.clear();
      type._templateValues.clear();
      type._pointers.clear();

      // initialize xml element parser
      XMLElementParser parser(xml);
      parser.addKeyword("name",true).addKeyword("namespace",false,true)
            .addKeyword("template",true,true).addKeyword("pointer",false,true)
            .addKeyword("ref",true).addKeyword("const",true).addKeyword("constexpr",true)
            .addKeyword("static",true);
      int element;

      // parse xml until end of root element is reached
      while ( ( element = parser() ) != XMLElementParser::End )
      {
         // determine which element is found and set data
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

      // make sure all required elements were read in
      if ( !parser.allRead() )
      {
         Exception::ReadError e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Failed reading in all required elements."));
         throw e;
      }

      // return reference to this
      return xml;
   }






   //@@
   QXmlStreamWriter& operator<<(QXmlStreamWriter& xml, const Type& type)
   {
      // write out name and all namespaces
      xml.writeTextElement("name",type._name);
      for (const auto& name : type._scope)
      {
         xml.writeTextElement("namespace",name);
      }

      // write start element for templates and initialize iterators
      xml.writeStartElement("template");
      auto templateArg {type._templateArguments.cbegin()};
      auto templateValue {type._templateValues.cbegin()};

      // iterate through all template arguments
      while ( templateArg != type._templateArguments.cend() )
      {
         // write out template argument and check if it has a value set
         xml.writeTextElement("arg",*templateArg);
         if ( templateValue != type._templateValues.cend() )
         {
            // write out value type within value element and increment iterator
            xml.writeStartElement("val");
            xml << *templateValue;
            xml.writeEndElement();
            ++templateValue;
         }

         // increment argument iterator
         ++templateArg;
      }

      // write end of template element
      xml.writeEndElement();

      // write our all pointers with constantness value for each one
      for (const auto& pointer : type._pointers)
      {
         xml.writeTextElement("pointer",QString::number(pointer));
      }

      // write out constantness and reference and return reference to this
      xml.writeTextElement("const",QString::number(type._valueConstant));
      xml.writeTextElement("constexpr",QString::number(type._expressionConstant));
      xml.writeTextElement("static",QString::number(type._static));
      xml.writeTextElement("ref",QString::number(type._reference));
      return xml;
   }
}
