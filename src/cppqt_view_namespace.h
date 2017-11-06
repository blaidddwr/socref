#ifndef CPPQT_NAMESPACEVIEW_H
#define CPPQT_NAMESPACEVIEW_H
#include <QLabel>



class AbstractBlock;



//@@
namespace CppQt
{
   //@@
   namespace View
   {
      //@@
      class Namespace : public QLabel
      {
      public:
         Namespace(AbstractBlock* block);
      };
   }
}



#endif
