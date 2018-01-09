#ifndef CPPQT_TYPEFACTORY_H
#define CPPQT_TYPEFACTORY_H
#include <memory>

#include "classes.h"
#include "singleton.h"



namespace CppQt
{
   class TypeFactory : public Singleton<TypeFactory>
   {
   public:
      enum
      {
         ConcreteType = 0
         ,TemplateType
         ,Total
      };
      std::unique_ptr<AbstractType> read(const QDomElement& type) const;
      QString name(int type) const;
   private:
      std::unique_ptr<AbstractType> makeType(int type) const;
   };
}



#endif
