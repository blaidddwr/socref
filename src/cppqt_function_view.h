#ifndef CPPQT_FUNCTION_VIEW_H
#define CPPQT_FUNCTION_VIEW_H
#include "cppqt_function.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the function block. This is designed so any inherited 
    * classes using the function block should not and cannot implement their own view. 
    */
   class Function::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Function* block);
   private:
      QString displayText();
      QString displayArguments();
      QString displayProperties();
      QString displayReturn();
      QString displayOperations();
      /*!
       * The function block that this view is displaying. 
       */
      const Function* _block;
   };
}



#endif
