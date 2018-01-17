#include "cppqt_abstracttype.h"
#include "exception.h"
#include "cppqt_type_factory.h"



using namespace CppQt;






QString AbstractType::scopedName(bool withModifiers) const
{
   QString ret;
   int count {0};
   for (int i = _scope.size() - 1; i >-1 ;--i)
   {
      if ( _depth == -1 || count++ < _depth )
      {
         ret.prepend("::").prepend(_scope.at(i));
      }
   }
   ret.append(fullName());
   if ( withModifiers )
   {
      ret = _modifiers.fullNameWithModifers(ret);
   }
   return ret;
}






AbstractType* AbstractType::setDepth(int depth)
{
   _depth = depth;
   return this;
}






AbstractType* AbstractType::setScope(const QStringList& scope)
{
   _scope = scope;
   return this;
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






AbstractType* AbstractType::setModifiers(const Type::Modifiers& modifiers)
{
   _modifiers = modifiers;
   return this;
}






QDomElement AbstractType::write(QDomDocument& document) const
{
   QDomElement ret {writeData(document)};
   ret.setAttribute("id",QString::number(type()));
   ret.setAttribute("type",Type::Factory::instance().name(type()));
   ret.setAttribute("scope",_scope.join("::"));
   ret.setAttribute("depth",QString::number(_depth));
   if ( !_modifiers.isNull() )
   {
      ret.setAttribute("mods",_modifiers.toString());
   }
   return ret;
}
