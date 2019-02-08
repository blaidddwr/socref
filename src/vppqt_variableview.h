#ifndef CPPQT_VARIABLE_VIEW_H
#define CPPQT_VARIABLE_VIEW_H
#include "cppqt_variable.h"
#include "basicblock_view.h"
//



namespace CppQt
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
      QString displayProperties();
      QString displayInitializer();
   private:
      /*!
       * The variable block that this view is displaying. 
       */
      const Variable* _block;
   };
}



#endif
