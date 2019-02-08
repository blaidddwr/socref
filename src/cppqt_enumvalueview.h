#ifndef CPPQT_ENUMVALUEVIEW_H
#define CPPQT_ENUMVALUEVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the enumeration value block. 
    */
   class EnumValueView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit EnumValueView(const EnumValue* block);
   private slots:
      QString displayValue();
   };
}



#endif
