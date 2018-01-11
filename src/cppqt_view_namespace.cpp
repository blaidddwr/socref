#include "cppqt_view_namespace.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace CppQt::View;






Namespace::Namespace(AbstractBlock* block, QWidget* parent):
   QLabel(parent)
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
   setText(QString("<h3>Description</h3><p>%1</p>").arg(block_->description()));
}
