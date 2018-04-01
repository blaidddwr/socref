#include "cppqt_view_type.h"
#include "cppqt_type.h"




using namespace CppQt::View;






Type::Type(const CppQt::Type* block, QWidget* parent):
   QLabel(parent),
   _block(block)
{
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
   setText(displayText());
}






QString Type::displayText()
{
   return _block->name().replace(",","&lt;");
}
