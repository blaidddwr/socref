#ifndef CPPQT_PARENT_VIEW_H
#define CPPQT_PARENT_VIEW_H
#include "cppqt_parent.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view widget for the parent block type. 
    */
   class Parent::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Parent* block);
   private:
      QString displayText();
      /*!
       * The parent block that this view is displaying. 
       */
      const Parent* _block;
   };
}



#endif
