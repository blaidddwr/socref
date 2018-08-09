#ifndef CPPQT_NAMESPACE_VIEW_H
#define CPPQT_NAMESPACE_VIEW_H
#include "cppqt_namespace.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the namespace block. 
    */
   class Namespace::View : public Base::View
   {
      Q_OBJECT
   public:
      explicit View(const Namespace* block);
   };
}



#endif
