#include "cppqt_abstracttype.h"
#include "exception.h"
#include "cppqt_typefactory.h"



using namespace CppQt;






QString AbstractType::scopedName(const QList<QString>& scope) const
{
   QString ret;
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
         ret.append(name).append("::");
      }
      ++count;
   }
   ret.append(fullName(scope));
   return ret;
}






AbstractType* AbstractType::clearScope()
{
   _scope.clear();
   return this;
}






AbstractType* AbstractType::prependScope(const QString& scope)
{
   if ( scope.isEmpty() )
   {
      return this;
   }
   if ( !QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*").exactMatch(scope) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot prepend invalid namespace '%1'.").arg(scope));
      throw e;
   }
   _scope.prepend(scope);
   return this;
}






AbstractType* AbstractType::setName(const QString& name)
{
   if ( !QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*").exactMatch(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set type name to '%1'.").arg(name));
      throw e;
   }
   _name = name;
   return this;
}






QDomElement AbstractType::write(QDomDocument& document)
{
   QDomElement ret {writeData(document)};
   ret.setAttribute("id",QString::number(type()));
   ret.setAttribute("type",TypeFactory::instance().name(type()));
   return ret;
}
