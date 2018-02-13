#include <QStringList>
#include "cppqt_view_base.h"
#include "exception.h"
#include "cppqt_base.h"




using namespace CppQt::View;






Base::Base(const AbstractBlock* block, QWidget* parent):
   QLabel(parent),
   _block(qobject_cast<const CppQt::Base*>(block))
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
   setMargin(8);
}






QString Base::displayText()
{
   return QString("<h2>Description</h2><p>").append(_block->description()).append("</p>");
}
