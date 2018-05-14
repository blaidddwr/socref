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
         explicit Access(const CppQt::Access* block, QWidget* parent = nullptr);
      protected:
         QString displayText();
      private:
         const CppQt::Access* _block;
      };
   }
}



#endif
