#ifndef CPPQT_FUNCTIONVIEW_H
#define CPPQT_FUNCTIONVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the function block. 
    */
   class FunctionView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit FunctionView(const Function* block);
   private slots:
      QString displayArguments();
      QString displayTemplates();
      QString displayProperties();
      QString displayReturn();
      QString displayOperations();
   };
}



#endif
