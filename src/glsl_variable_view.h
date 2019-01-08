#ifndef GLSL_VARIABLE_VIEW_H
#define GLSL_VARIABLE_VIEW_H
#include "glsl_variable.h"
#include "basicblock_view.h"
//



namespace GLSL
{
   /*!
    * This is the view class for the variable block. 
    */
   class Variable::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const Variable* block);
   private slots:
      QString displayLayout();
   private:
      /*!
       * Pointer to the variable block that this view displays. 
       */
      const Variable* _block;
   };
}



#endif
