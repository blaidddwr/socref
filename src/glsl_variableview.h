#ifndef GLSL_VARIABLEVIEW_H
#define GLSL_VARIABLEVIEW_H
#include "basic_blockview.h"
#include "glsl.h"
//



namespace GLSL
{
   /*!
    * This is the view class for the variable block. 
    */
   class VariableView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit VariableView(const Variable* block);
   private slots:
      QString displayLayout();
   };
}



#endif
