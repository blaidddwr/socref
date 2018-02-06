#include "cppqt_view_variable.h"



using namespace CppQt::View;






Variable::Variable(const AbstractBlock* block, bool wait, QWidget* parent):
   Base(block,parent)
{
   if ( !wait ) setText(displayText());
}






QString Variable::displayText()
{
   return Base::displayText();
}
