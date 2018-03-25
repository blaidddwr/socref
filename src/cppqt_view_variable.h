#ifndef CPPQT_VIEW_VARIABLE_H
#define CPPQT_VIEW_VARIABLE_H
#include "cppqt_view_base.h"



namespace CppQt
{
   namespace View
   {
      class Variable : public Base
      {
         Q_OBJECT
      public:
         explicit Variable(const AbstractBlock* block, bool wait = false, QWidget* parent = nullptr);
      protected:
         QString displayText();
         QString displayType();
         QString displayProperties(QStringList list);
         QStringList getProperties();
      private:
         QString displayInitializer();
         const CppQt::Variable* _block;
      };
   }
}



#endif
