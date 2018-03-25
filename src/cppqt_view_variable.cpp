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
   return displayType().append(Base::displayText())
                       .append(displayProperties(getProperties()))
                       .append(displayInitializer());
}






QString Variable::displayType()
{
   QString ret {"<h3>Type</h3><p>"};
   ret.append(_block->variableType().replace("<","&lt;")).append("</p>");
   return ret;
}






QString Variable::displayProperties(QStringList list)
{
   QString ret;
   if ( !list.isEmpty() )
   {
      ret.append("<h3>Properties</h3><ul><li>").append(list.join("</li><li>")).append("</li></ul>");
   }
   return ret;
}






QStringList Variable::getProperties()
{
   QStringList ret;
   if ( _block->isConstExpr() ) ret << "constexpr";
   if ( _block->isStatic() ) ret << "static";
   if ( _block->isMutable() ) ret << "mutable";
   return ret;
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
