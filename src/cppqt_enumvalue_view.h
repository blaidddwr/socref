#ifndef CPPQT_ENUMVALUE_VIEW_H
#define CPPQT_ENUMVALUE_VIEW_H
#include "cppqt_enumvalue.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the enumeration value block. 
    */
   class EnumValue::View : public Base::View
   {
      Q_OBJECT
   public:
      explicit View(const EnumValue* block);
   private:
      QString displayText();
      QString displayValue();
      /*!
       * Pointer to the enumeration value block this view is showing. 
       */
      const EnumValue* _block;
   };
}



#endif
