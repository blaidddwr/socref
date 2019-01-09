#ifndef GLSL_FUNCTION_VIEW_H
#define GLSL_FUNCTION_VIEW_H
#include "glsl_function.h"
#include "basicblock_view.h"
//



namespace GLSL
{
   /*!
    * This is the view class for the function block. 
    */
   class Function::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const Function* block);
   private slots:
      QString displayArguments();
      QString displayReturn();
      QString displayOperations();
   private:
      /*!
       * Pointer to the function block that this view displays. 
       */
      const Function* _block;
   };
}



#endif
