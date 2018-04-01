#include "cppqt_view_base.h"
#include <QStringList>
#include "cppqt_base.h"




using namespace CppQt::View;






Base::Base(const CppQt::Base* block, bool wait, QWidget* parent):
   QLabel(parent),
   _block(block)
{
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setMargin(8);
   if ( !wait ) setText(displayText());
}






QString Base::displayText()
{
   return QString("<h3>Description</h3><p>").append(_block->description()).append("</p>");
}
