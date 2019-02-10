#ifndef CPPQT_ACCESSVIEW_H
#define CPPQT_ACCESSVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the access block. This provides a custom display that 
    * shows a tally count of different block types this view's access block contains 
    * as children blocks. 
    */
   class AccessView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit AccessView(const Access* block);
   private slots:
      QString displayInfo();
   };
}



#endif
