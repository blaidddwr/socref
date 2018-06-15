#ifndef CPPQT_USING_VIEW_H
#define CPPQT_USING_VIEW_H
#include "cppqt_using.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    */
   class Using::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Using* block);
   private:
      QString displayText();
      /*!
       * The variable block that this view is displaying. 
       */
      const Using* _block;
   };
}



#endif
