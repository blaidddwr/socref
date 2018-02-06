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
         explicit Access(const AbstractBlock* block, QWidget* parent = nullptr);
      private:
         void bodyChanged();
         const CppQt::Access* _block;
      };
   }
}



#endif
