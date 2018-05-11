#include "cppqt_view_base.h"
#include <QStringList>
#include "cppqt_view_common.h"
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
   QString ret {"<h3>Description</h3><p>"};
   QStringList paragraphs {_block->description().split("\n\n",QString::SkipEmptyParts)};
   for (int i = 0; i < (paragraphs.size() - 1) ;++i)
   {
      ret.append(parseBoldMarkers(paragraphs.at(i))).append("</p><p>");
   }
   if ( !paragraphs.isEmpty() ) ret.append(paragraphs.last()).append("</p>");
   return ret;
}
