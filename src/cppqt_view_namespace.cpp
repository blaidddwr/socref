#include "cppqt_view_namespace.h"
#include "abstractblock.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace CppQt::View;






Namespace::Namespace(AbstractBlock* block)
{
   CppQt::Namespace* block_ {qobject_cast<CppQt::Namespace*>(block)};
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
   setText(QString("<p>namespace <b>%1</b></p><p>%2</p>").arg(block_->name()).arg(block_->description()));
}
