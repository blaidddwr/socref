#include "exception.h"
#include "cppqt_namespaceview.h"
#include "cppqt_namespace.h"



using namespace CppQt;






//@@
NamespaceView::NamespaceView(AbstractBlock* block)
{
   // get namespace block pointer from abstract one and make sure it worked
   Namespace* block_ {qobject_cast<Namespace*>(block)};
   if ( !block_ )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }

   // set attributes of label
   setAlignment(Qt::AlignTop);
   setWordWrap(true);
   setTextFormat(Qt::RichText);

   // set label text
   setText(QString("<p>namespace <b>%1</b></p><p>%2</p>").arg(block_->getName())
           .arg(block_->getDescription()));
}
