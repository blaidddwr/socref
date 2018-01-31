#ifndef CPPQT_VIEW_FUNCTION_H
#define CPPQT_VIEW_FUNCTION_H
#include "cppqt_view_base.h"



namespace CppQt
{
   namespace View
   {
      class Function : public Base
      {
         Q_OBJECT
      public:
         explicit Function(AbstractBlock* block, bool wait = false, QWidget* parent = nullptr);
      protected:
         QString displayText();
         QString displayArgumentsText();
         QString displayReturnText();
         QString displayOperationsText();
      private:
         CppQt::Function* _block;
      };
   }
}



#endif
