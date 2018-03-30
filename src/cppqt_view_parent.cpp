#include "cppqt_view_parent.h"
#include "cppqt_parent.h"



using namespace CppQt::View;






Parent::Parent(const CppQt::Parent* block, QWidget* parent):
   QLabel(parent),
   _block(block)
{
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
   setText(displayText());
}






QString Parent::displayText()
{
   return QString("<b>").append(_block->accessName())
                        .append("</b> ")
                        .append(_block->className().replace("<","&lt;"));
}
