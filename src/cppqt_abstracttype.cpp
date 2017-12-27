#include "cppqt_abstracttype.h"
#include "exception.h"






QString AbstractType::name(const QList<QString> scope) const
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
   ret.append(rawName());
   return ret;
}






void AbstractType::setScope(const QList<QString> scope)
{
   QRegExp regexp("[a-zA-Z_]+[a-zA-Z0-9_]*");
   for (const auto& name : _scope)
   {
      if ( !regexp.exactMatch(name) )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Cannot set invalid namespace '%1'.").arg(name));
         throw e;
      }
   }
   _scope = scope;
}
