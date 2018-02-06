#ifndef CPPQT_VIEW_ACCESS_H
#define CPPQT_VIEW_ACCESS_H
#include <QLabel>
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace View
   {
      class Access : public QLabel
      {
         Q_OBJECT
      public:
         explicit Access(AbstractBlock* block, QWidget* parent = nullptr);
      private:
         void bodyChanged();
         CppQt::Access* _block;
      };
   }
}



#endif
