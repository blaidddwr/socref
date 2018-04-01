#ifndef CPPQT_VIEW_TYPE_H
#define CPPQT_VIEW_TYPE_H
#include <QLabel>
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace View
   {
      class Type : public QLabel
      {
         Q_OBJECT
      public:
         explicit Type(const CppQt::Type* block, QWidget* parent = nullptr);
      protected:
         QString displayText();
      private:
         const CppQt::Type* _block;
      };
   }
}



#endif
