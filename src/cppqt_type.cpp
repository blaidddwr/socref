#include <QRegExp>

#include "cppqt_type.h"
#include "exception.h"



using namespace CppQt;






//@@
QString Type::getName(const QList<QString>& scope) const
{
   // create return string
   QString ret;

   // if type is constant value add it to string
   if ( _constantValue )
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
   ret.append(_name).append(_pointer);

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

   // if type is constant pointer append it to string
   if ( _constantPointer )
   {
      ret.append(" const");
   }

   // return constructed type name
   return ret;
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
Type& Type::setPointer(const QString& pointer)
{
   // make sure pointer string is correct
   if ( !QRegExp("\\**\\&?").exactMatch(pointer) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Invalid pointer string '%1' given for deriving new type.")
                   .arg(pointer));
      throw e;
   }

   // if constant pointer is set make sure pointer type is of correct type
   if ( _constantPointer && !QRegExp("\\*+").exactMatch(pointer) )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set type as constant pointer with pointer string '%1'.")
                   .arg(pointer));
      throw e;
   }

   // set new pointer string and return reference to this
   _pointer = pointer;
   return *this;
}






//@@
Type& Type::setConstants(bool constantValue, bool constantPointer)
{
   // if constant pointer is set make sure pointer type is of correct type
   if ( constantPointer && !QRegExp("\\*+").exactMatch(_pointer) )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set type as constant pointer with pointer string '%1'.")
                   .arg(_pointer));
      throw e;
   }

   // set constant values and return reference to this
   _constantValue = constantValue;
   _constantPointer = constantPointer;
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
