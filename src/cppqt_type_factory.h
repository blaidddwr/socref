#ifndef CPPQT_TYPEFACTORY_H
#define CPPQT_TYPEFACTORY_H
#include <memory>
#include <QString>
#include "singleton.h"
#include "cppqt_classes.h"



class QDomElement;



namespace CppQt
{
   namespace Type
   {
      class Factory : public Singleton<Factory>
      {
      public:
         enum
         {
            ConcreteType = 0
            ,TemplateType
            ,Total
         };
         std::unique_ptr<AbstractType> read(const QDomElement& type) const;
         bool isValidTypeElement(const QDomElement& element);
         QString name(int type) const;
      private:
         std::unique_ptr<AbstractType> makeType(int type) const;
      };
   }
}



#endif
