#ifndef CPPQT_TYPEFACTORY_H
#define CPPQT_TYPEFACTORY_H
#include <memory>

#include "singleton.h"
#include "cppqt_abstracttype.h"
#include "cppqt_type_concrete.h"
#include "cppqt_type_template.h"



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
      std::unique_ptr<AbstractType> read(QXmlStreamReader& xml) const;
      QString name(int type) const;
   private:
      std::unique_ptr<AbstractType> makeType(int type) const;
   };
}



#endif
