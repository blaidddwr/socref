#include "cppqt_view_namespace.h"
#include "cppqt_namespace.h"
#include "exception.h"



using namespace CppQt::View;






Namespace::Namespace(AbstractBlock* block, bool wait, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<CppQt::Namespace*>(block))
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
   return Base::displayText().append(displayTypes());
}






QString Namespace::displayTypes()
{
   QString ret;
   const QStringList local {_block->types()};
   const QStringList global {_block->root()->types()};
   if ( !local.isEmpty() || !global.isEmpty() )
   {
      ret.append("<h2>Types</h2>");
      if ( !local.isEmpty() )
      {
         ret.append("<h3>Local</h3>");
         for (int i = 0; i < (local.size() - 1) ;++i) ret.append(local.at(i)).append("<br/>");
         ret.append(local.back());
      }
      if ( !global.isEmpty() )
      {
         ret.append("<h3>Global</h3>");
         for (int i = 0; i < (global.size() - 1) ;++i) ret.append(global.at(i)).append("<br/>");
         ret.append(global.back());
      }
   }
   return ret;
}
