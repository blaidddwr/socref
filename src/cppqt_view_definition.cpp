#include "cppqt_view_definition.h"
#include "cppqt_definition.h"
#include "exception.h"



using namespace CppQt::View;






//@@
Definition::Definition(AbstractBlock* block)
{
   // get namespace block pointer from abstract one and make sure it worked
   CppQt::Definition* block_ {qobject_cast<CppQt::Definition*>(block)};
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
   setText(QString("<p>").append(block_->getName()).append("</p>"));
}
