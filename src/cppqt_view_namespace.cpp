#include "cppqt_view_namespace.h"
#include "cppqt_namespace.h"



using namespace CppQt::View;






//@@
Namespace::Namespace(AbstractBlock *block)
{
   setText(QString("<h3>%1</h3><p>%2</p>").arg(block->getName()).arg("not yet"));
}
