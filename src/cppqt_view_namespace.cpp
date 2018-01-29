#include "cppqt_view_namespace.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace CppQt::View;






Namespace::Namespace(AbstractBlock* block, bool wait, QWidget* parent):
   Base(block,parent)
{
   if ( !wait )
   {
      setText(Base::displayText());
   }
}






QString Namespace::displayText()
{
   return Base::displayText();
}
