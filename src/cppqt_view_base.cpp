#include <QStringList>
#include "cppqt_view_base.h"
#include "exception.h"
#include "cppqt_base.h"




using namespace CppQt::View;






Base::Base(AbstractBlock* block, QWidget* parent):
   QLabel(parent),
   _block(qobject_cast<CppQt::Base*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
}






QString Base::displayText()
{
   return QString("<h1>Description</h1><p>").append(_block->description().split("\n\n").join("</p><p>")).append("</p>");
}
