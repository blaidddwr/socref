#ifndef CPPQT_VIEW_PARENT_H
#define CPPQT_VIEW_PARENT_H
#include <QLabel>
#include "cppqt.h"



namespace CppQt
{
   namespace View
   {
      class Parent : public QLabel
      {
         Q_OBJECT
      public:
         explicit Parent(const CppQt::Parent* block, QWidget* parent = nullptr);
      private:
         QString displayText();
         const CppQt::Parent* _block;
      };
   }
}



#endif
