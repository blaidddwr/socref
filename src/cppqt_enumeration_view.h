#ifndef CPPQT_ENUMERATION_VIEW_H
#define CPPQT_ENUMERATION_VIEW_H
#include "cppqt_enumeration.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the enumeration block. 
    */
   class Enumeration::View : public Base::View
   {
      Q_OBJECT
   public:
      explicit View(const Enumeration* block);
   };
}



#endif
