#include "cppqt_abstractdefinition.h"
#include "cppqt_abstracttype.h"



using namespace CppQt;






AbstractType* AbstractDefinition::type()
{
   return rawType()->clearScope();
}
