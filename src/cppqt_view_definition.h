#ifndef CPPQT_VIEW_DEFINITION_H
#define CPPQT_VIEW_DEFINITION_H
#include <QLabel>

#include "classes.h"



namespace CppQt
{
   namespace View
   {
      class Definition : public QLabel
      {
         Q_OBJECT
      public:
         explicit Definition(AbstractBlock* block, QWidget* parent = nullptr);
      };
   }
}



#endif
