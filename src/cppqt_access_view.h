#ifndef CPPQT_ACCESS_VIEW_H
#define CPPQT_ACCESS_VIEW_H
#include "cppqt_access.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    */
   class Access::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Access* block);
   private:
      QString displayText();
      /*!
       * The variable block that this view is displaying. 
       */
      const Access* _block;
   };
}



#endif
