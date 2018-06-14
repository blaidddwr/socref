#ifndef CPPQT_TEMPLATE_VIEW_H
#define CPPQT_TEMPLATE_VIEW_H
#include "cppqt_template.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the template block. 
    */
   class Template::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Template* block);
   };
}



#endif
