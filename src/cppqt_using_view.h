#ifndef CPPQT_USING_VIEW_H
#define CPPQT_USING_VIEW_H
#include "cppqt_using.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view widget for the using block type. 
    */
   class Using::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Using* block);
   private:
      QString displayText();
      /*!
       * The using block that this view is displaying. 
       */
      const Using* _block;
   };
}



#endif
