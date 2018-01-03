#ifndef CPPQT_NAMESPACEVIEW_H
#define CPPQT_NAMESPACEVIEW_H
#include <QLabel>

#include "abstractblock.h"



namespace CppQt
{
   namespace View
   {
      class Namespace : public QLabel
      {
         Q_OBJECT
      public:
         explicit Namespace(AbstractBlock* block);
      };
   }
}



#endif
