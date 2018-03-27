#ifndef CPPQT_VIEW_NAMESPACE_H
#define CPPQT_VIEW_NAMESPACE_H
#include "cppqt_view_base.h"



namespace CppQt
{
   namespace View
   {
      class Namespace : public Base
      {
         Q_OBJECT
      public:
         explicit Namespace(const CppQt::Namespace* block, bool wait = false, QWidget* parent = nullptr);
      };
   }
}



#endif
