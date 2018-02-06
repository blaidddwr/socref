#ifndef CPPQT_NAMESPACEVIEW_H
#define CPPQT_NAMESPACEVIEW_H
#include "cppqt_view_base.h"



namespace CppQt
{
   namespace View
   {
      class Namespace : public Base
      {
         Q_OBJECT
      public:
         explicit Namespace(const AbstractBlock* block, bool wait = false, QWidget* parent = nullptr);
      protected:
         QString displayText();
      private:
         const CppQt::Namespace* _block;
      };
   }
}



#endif
