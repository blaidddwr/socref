#include "cppqt_view_definition.h"
#include "cppqt_definition.h"
#include "exception.h"



using namespace CppQt::View;






Definition::Definition(AbstractBlock* block)
{
   CppQt::Definition* block_ {qobject_cast<CppQt::Definition*>(block)};
   if ( !block_ )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);
   setText(QString("<p>").append(block_->getName()).append("</p>"));
}
