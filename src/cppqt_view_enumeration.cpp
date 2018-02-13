#include "cppqt_view_enumeration.h"



using namespace CppQt::View;






Enumeration::Enumeration(const AbstractBlock* block, QWidget* parent):
   Base(block,parent)
{
   setText(displayText());
}
