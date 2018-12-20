#ifndef CPPQT_FUNCTION_VIEW_H
#define CPPQT_FUNCTION_VIEW_H
#include "cppqt_function.h"
#include "basicblock_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the function block. This is designed so any inherited 
    * classes using the function block should not and cannot implement their own view. 
    */
   class Function::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const Function* block);
   private slots:
      QString displayArguments();
      QString displayTemplates();
      QString displayProperties();
      QString displayReturn();
      QString displayOperations();
   private:
      /*!
       * The function block that this view is displaying. 
       */
      const Function* _block;
   };
}



#endif
