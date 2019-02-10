#ifndef CPPQT_VARIABLEVIEW_H
#define CPPQT_VARIABLEVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the variable block. This provides custom displays 
    * that show the properties and initializer of this view's variable, if any for 
    * either one. 
    */
   class VariableView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit VariableView(const Variable* block);
   private slots:
      QString displayProperties();
      QString displayInitializer();
   };
}



#endif
