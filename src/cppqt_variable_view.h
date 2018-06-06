#ifndef CPPQT_VARIABLE_VIEW_H
#define CPPQT_VARIABLE_VIEW_H
#include "cppqt_variable.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the variable block. 
    */
   class Variable::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Variable* block);
   private:
      QString displayText();
      QString displayType();
      QString displayProperties();
      QString displayInitializer();
      /*!
       * The variable block that this view is displaying. 
       */
      const Variable* _block;
   };
}



#endif
