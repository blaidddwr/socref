#ifndef CPPQT_BASE_H
#define CPPQT_BASE_H
#include "basicblock.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the base block class used by many other concrete block types within the 
    * C++/Qt project type. This base class implements returning a list of possible 
    * types this block can assume, such as a variable type or function return type, 
    * because C++ is a hardly typed language. 
    */
   class Base : public BasicBlock
   {
      Q_OBJECT
   protected:
      virtual QJsonObject typeList() const override final;
   private:
      static QJsonObject addList(const TypeList& block);
   };
}



#endif
