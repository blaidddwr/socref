#include "cppqt_view_variable.h"
#include <exception.h>
#include "cppqt_variable.h"



using namespace CppQt::View;






Variable::Variable(const AbstractBlock* block, bool wait, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<const CppQt::Variable*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   if ( !wait ) setText(displayText());
}






QString Variable::displayText()
{
   return Base::displayText().append(displayInitializer());
}






QString Variable::displayInitializer()
{
   QString ret;
   if ( _block->hasInitializer() )
   {
      const QStringList parts {_block->initializer().split(",")};
      ret.append("<h3>Initializer</h3>");
      for (auto part : parts) ret.append(part).append("<br/>");
   }
   return ret;
}
