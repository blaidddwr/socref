#ifndef CPPQT_ABSTRACTDEFINITION_H
#define CPPQT_ABSTRACTDEFINITION_H
#include "cppqt_abstracttype.h"
#include "cppqt_abstracttype.h"



namespace CppQt
{
   class AbstractDefinition
   {
   public:
      AbstractType* type() { return rawType()->clearScope(); }
   protected:
      virtual AbstractType* rawType() = 0;
   };
}



#endif
