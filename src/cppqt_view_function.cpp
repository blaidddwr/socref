#include "cppqt_view_function.h"
#include <exception.h>
#include "cppqt_view_common.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"



using namespace CppQt::View;






Function::Function(const AbstractBlock* block, bool wait, QWidget* parent):
   Variable(block,parent),
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
   return displayArgumentsText()
         .append(displayProperties(getProperties()))
         .append(displayTemplatesText())
         .append(Base::displayText())
         .append(displayReturnText())
         .append(displayOperationsText());
}






QString Function::displayTemplatesText()
{
   return View::displayTemplatesText(_block);
}






QString Function::displayArgumentsText()
{
   QString ret;
   const QList<CppQt::Variable*> list {_block->arguments()};
   if ( !list.isEmpty() )
   {
      ret.append("<h3>Arguments</h3>");
      for (auto variable : list)
      {
         ret
               .append("<p>")
               .append(variable->variableType().replace("<","&lt;"))
               .append(" <b>")
               .append(variable->Base::name())
               .append("</b>");
         if ( variable->hasInitializer() ) ret.append(" = ").append(variable->initializer());
         ret.append(" : ").append(variable->description()).append("</p>");
      }
   }
   return ret;
}






QString Function::displayReturnText()
{
   QString ret;
   if ( !_block->returnDescription().isEmpty() )
   {
      ret.append("<h3>Return</h3><p><b>").append(_block->returnType()).append(" :</b> ");
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
      ret.append("<h3>Operations</h3>");
      for (int i = 0; i < list.size() ;++i)
      {
         ret.append("<p><b>#")
            .append(QString::number(i + 1))
            .append("</b> ")
            .append(list.at(i))
            .append("</p>");
      }
   }
   return ret;
}






QStringList Function::getProperties()
{
   QStringList ret {Variable::getProperties()};
   if ( _block->isVirtual() ) ret << "virtual";
   if ( _block->isConst() ) ret << "const";
   if ( _block->isNoExcept() ) ret << "noexcept";
   if ( _block->isOverride() ) ret << "override";
   if ( _block->isFinal() ) ret << "final";
   if ( _block->isAbstract() ) ret << "abstract(= 0)";
   return ret;
}






void Function::bodyChanged()
{
   setText(displayText());
}
