#include "cppqt_abstractcontainer.h"
#include "cppqt_abstractdefinition.h"
#include "cppqt_abstracttype.h"



using namespace CppQt;






const QList<AbstractType*> AbstractContainer::contains()
{
   QList<AbstractType*> ret;
   QString scope {scopeName()};
   for (int i = 0; i < childrenSize() ;++i)
   {
      if ( AbstractContainer* container = qobject_cast<AbstractContainer*>(child(i)) )
      {
         const QList<AbstractType*> sub {container->contains()};
         for (auto type : sub)
         {
            type->prependScope(scope);
         }
         ret.append(sub);
      }
      if ( AbstractDefinition* definition = dynamic_cast<AbstractDefinition*>(child(i)) )
      {
         ret.append(definition->type()->prependScope(scope));
      }
   }
   return ret;
}






bool AbstractContainer::exists(const QList<AbstractType*>& allTypes, const QList<AbstractType*>& types)
{
   for (auto type : types)
   {
      bool found {false};
      for (auto subType : allTypes)
      {
         if ( *subType == *type && exists(allTypes,type->contains()) )
         {
            found = true;
            break;
         }
      }
      if ( found )
      {
         break;
      }
      return false;
   }
   return true;
}
