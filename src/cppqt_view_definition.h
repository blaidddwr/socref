#ifndef CPPQT_VIEW_DEFINITION_H
#define CPPQT_VIEW_DEFINITION_H
#include <QLabel>



class AbstractBlock;



//@@
namespace CppQt
{
   //@@
   namespace View
   {
      //@@
      class Definition : public QLabel
      {
      public:
         Definition(AbstractBlock* block);
      };
   }
}



#endif
