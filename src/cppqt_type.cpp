#include <QRegExp>

#include "cppqt_type.h"
#include "exception.h"



using namespace CppQt;






//@@
Type Type::toType(const QString& pointer, bool constantValue, bool constantPointer) const
{
   // make sure pointer string is correct
   QRegExp regexp("\\**\\&?");
   if ( !regexp.exactMatch(pointer) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Invalid pointer string '%1' given for deriving new type.")
                   .arg(pointer));
      throw e;
   }

   // if constant pointer is requested make sure pointer type is of correct type
   regexp = QRegExp("\\*+");
   if ( constantPointer && !regexp.exactMatch(pointer) )
   {
      Exception::InvalidUse e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set type as constant pointer with pointer string '%1'.")
                   .arg(pointer));
      throw e;
   }

   // create new type from this type's data
   Type ret(_scope,_name,_templateArguments);

   // set new data to new type and return it
   ret._pointer = pointer;
   ret._constantValue = constantValue;
   ret._constantPointer = constantPointer;
   return ret;
}






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

   // check if type has any template arguments
   if ( !_templateArguments.isEmpty() )
   {
      // iterate through all template arguments and append it to type name string
      ret.append("<");
      for (int i = 0; i < (_templateArguments.size() - 1) ;++i)
      {
         ret.append(_templateArguments.at(i).getName(scope)).append(",");
      }
      ret.append(_templateArguments.constLast().getName(scope)).append(">");
   }

   // if type is constant pointer append it to string
   if ( _constantPointer )
   {
      ret.append(" const");
   }

   // return constructed type name
   return ret;
}
