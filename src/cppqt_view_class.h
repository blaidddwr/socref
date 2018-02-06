#ifndef CPPQT_VIEW_CLASS_H
#define CPPQT_VIEW_CLASS_H
#include "cppqt_view_namespace.h"



namespace CppQt
{
   namespace View
   {
      class Class : public Namespace
      {
         Q_OBJECT
      public:
         explicit Class(const AbstractBlock* block, QWidget* parent = nullptr);
      protected:
         QString displayText();
         QString displayQtObjectText();
         QString displayTemplatesText();
      private slots:
         void bodyChanged();
      private:
         const CppQt::Class* _block;
      };
   }
}



#endif
