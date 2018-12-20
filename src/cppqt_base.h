#ifndef CPPQT_BASE_H
#define CPPQT_BASE_H
#include "basicblock.h"
//



namespace CppQt
{
   /*!
    * This is the base block class used by many other concrete block types within the 
    * C++/Qt project type. This base class provides methods for getting the base name 
    * and description of a block which almost all block types share. 
    */
   class Base : public BasicBlock
   {
      Q_OBJECT
   public:
      QString baseName() const;
      QString description() const;
   };
}



#endif
