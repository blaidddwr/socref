#include "cppqt_view_enumvalue.h"



using namespace CppQt::View;






EnumValue::EnumValue(const AbstractBlock* block, QWidget* parent):
   Base(block,parent)
{
   setText(displayText());
}
