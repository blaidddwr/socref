#ifndef CPPQT_VIEW_DECLARATION_H
#define CPPQT_VIEW_DECLARATION_H
#include <QLabel>
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace View
   {
      class Declaration : public QLabel
      {
      public:
         Declaration(const AbstractBlock* block);
      private:
         QString displayText();
         const CppQt::Declaration* _block;
      };
   }
}



#endif
