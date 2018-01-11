#include "cppqt_view_definition.h"
#include "cppqt_definition.h"
#include "exception.h"



using namespace CppQt::View;






Definition::Definition(AbstractBlock* block, QWidget* parent):
   QLabel(parent)
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
   AbstractType* type {block_->getType()};
   if ( type->isConcrete() )
   {
      setText(tr("Concrete"));
   }
   else if ( type->isTemplate() )
   {
      setText(tr("Template"));
   }
   else
   {
      setText(tr("Unknown"));
   }
}
