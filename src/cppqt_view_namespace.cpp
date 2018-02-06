#include "cppqt_view_namespace.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace CppQt::View;






Namespace::Namespace(const AbstractBlock* block, bool wait, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<const CppQt::Namespace*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   if ( !wait )
   {
      setText(displayText());
   }
}






QString Namespace::displayText()
{
   return Base::displayText();
}
