#ifndef CPPQT_VIEW_BASE_H
#define CPPQT_VIEW_BASE_H
#include <QLabel>
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace View
   {
      class Base : public QLabel
      {
         Q_OBJECT
      public:
         explicit Base(AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         QString displayText();
      private:
         CppQt::Base* _block;
      };
   }
}



#endif
