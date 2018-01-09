#ifndef CPPQT_ABSTRACTDEFINITION_H
#define CPPQT_ABSTRACTDEFINITION_H
#include "classes.h"



namespace CppQt
{
   class AbstractDefinition
   {
   public:
      AbstractType* type();
   protected:
      virtual AbstractType* rawType() = 0;
   };
}



#endif
