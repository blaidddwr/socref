#include "cppqt_view_function.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "exception.h"
#include "cppqt_template.h"
#include "cppqt_view_common.h"



using namespace CppQt::View;






Function::Function(const AbstractBlock* block, bool wait, QWidget* parent):
   Base(block,parent),
   _block(qobject_cast<const CppQt::Function*>(block))
{
   if ( !_block )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Abstract block is not correct type."));
      throw e;
   }
   connect(_block,&CppQt::Function::bodyChanged,this,&Function::bodyChanged);
   if ( !wait ) setText(displayText());
}






QString Function::displayText()
{
   return Base::displayText().append(displayTemplatesText()).append(displayArgumentsText()).append(displayReturnText()).append(displayOperationsText());
}






QString Function::displayTemplatesText()
{
   return View::displayTemplatesText(_block);
}






QString Function::displayArgumentsText()
{
   QString ret;
   const QList<Variable*> list {_block->arguments()};
   if ( !list.isEmpty() )
   {
      ret.append("<h2>Arguments</h2>");
      for (auto variable : list)
      {
         ret.append("<p>").append(variable->variableType()).append(" <b>").append(variable->Base::name()).append("</b> : ");
         ret.append(variable->description()).append("</p>");
      }
   }
   return ret;
}






QString Function::displayReturnText()
{
   QString ret;
   if ( !_block->returnDescription().isEmpty() )
   {
      ret.append("<h2>Return</h2><p><b>").append(_block->returnType()).append(" :</b> ");
      ret.append(_block->returnDescription()).append("</p>");
   }
   return ret;
}






QString Function::displayOperationsText()
{
   QString ret;
   const QStringList list {_block->operations()};
   if ( !list.isEmpty() )
   {
      ret.append("<h2>Operations</h2><ol>");
      for (auto operation : list) ret.append("<li>").append(operation).append("</li>");
      ret.append("</ol>");
   }
   return ret;
}






void Function::bodyChanged()
{
   setText(displayText());
}
