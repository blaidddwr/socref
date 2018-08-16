#ifndef CPPQT_TYPE_VIEW_H
#define CPPQT_TYPE_VIEW_H
#include "cppqt_type.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the type block. 
    */
   class Type::View : public Base::View
   {
      Q_OBJECT
   public:
      explicit View(const Type* block);
   private:
      QString displayText();
      /*!
       * Pointer to the type block this view is showing. 
       */
      const Type* _block;
   };
}



#endif
