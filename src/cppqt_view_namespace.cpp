#include "cppqt_view_namespace.h"
#include <exception.h>
#include "cppqt_namespace.h"



using namespace CppQt::View;






Namespace::Namespace(const CppQt::Namespace* block, bool wait, QWidget* parent):
   Base(block,parent)
{
   if ( !wait ) setText(Base::displayText());
}
