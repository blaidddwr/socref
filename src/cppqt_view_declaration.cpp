#include "cppqt_view_declaration.h"
#include <exception.h>
#include "cppqt_declaration.h"



using namespace CppQt::View;






Declaration::Declaration(const AbstractBlock* block):
   _block(qobject_cast<const CppQt::Declaration*>(block))
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
   setText(displayText());
}






QString Declaration::displayText()
{
   return QString("<b>").append(_block->declareTypeName()).append("</b> ").append(_block->field().replace("<","&lt;"));
}
