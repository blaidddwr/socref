#include "cppqt_view_parent.h"



using namespace CppQt::View;






Parent::Parent(const AbstractBlock* block, QWidget* parent):
   Base(block,parent)
{
   setText(displayText());
}
