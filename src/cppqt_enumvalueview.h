#ifndef CPPQT_ENUMVALUEVIEW_H
#define CPPQT_ENUMVALUEVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * This is the view class for the enumeration value block. This provides a
    * custom display that shows the value of this view's enumeration value or an
    * empty string if there is no value.
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
