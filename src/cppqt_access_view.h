#ifndef CPPQT_ACCESS_VIEW_H
#define CPPQT_ACCESS_VIEW_H
#include "cppqt_access.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view widget for the access block type. 
    */
   class Access::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Access* block);
   private:
      QString displayText();
      /*!
       * The access block that this view is displaying. 
       */
      const Access* _block;
   };
}



#endif
