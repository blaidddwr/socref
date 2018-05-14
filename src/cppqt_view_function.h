#ifndef CPPQT_VIEW_FUNCTION_H
#define CPPQT_VIEW_FUNCTION_H
#include "cppqt_view_variable.h"



namespace CppQt
{
   namespace View
   {
      class Function : public Variable
      {
         Q_OBJECT
      public:
         explicit Function(const CppQt::Function* block, bool wait = false, QWidget* parent = nullptr);
      protected:
         QString displayText();
         QString displayTemplatesText();
         QString displayArgumentsText();
         QString displayReturnText();
         QString displayOperationsText();
         QStringList getProperties();
      private:
         const CppQt::Function* _block;
      };
   }
}



#endif
