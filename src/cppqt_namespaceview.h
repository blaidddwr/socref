#ifndef CPPQT_NAMESPACEVIEW_H
#define CPPQT_NAMESPACEVIEW_H
#include <QLabel>



class AbstractBlock;



//@@
namespace CppQt
{
   //@@
   class NamespaceView : public QLabel
   {
   public:
      NamespaceView(AbstractBlock* block);
   };
}



#endif
