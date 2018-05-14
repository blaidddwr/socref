#ifndef CPPQT_VIEW_TYPELIST_H
#define CPPQT_VIEW_TYPELIST_H
#include <QLabel>
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace View
   {
      class TypeList : public QLabel
      {
         Q_OBJECT
      public:
         explicit TypeList(const CppQt::TypeList* block, QWidget* parent = nullptr);
      protected:
         QString displayText();
      private:
         QString displayTypeLists();
         QString displayTypes();
         const CppQt::TypeList* _block;
      };
   }
}



#endif
