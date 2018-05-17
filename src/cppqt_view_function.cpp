#include "cppqt_view_function.h"
#include <exception.h>
#include "cppqt_view_common.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"



using namespace CppQt::View;






Function::Function(const CppQt::Function* block, bool wait, QWidget* parent):
   Variable(block,true,parent),
   _block(block)
{
   if ( !wait ) setText(displayText());
}






QString Function::displayText()
{
   return displayArgumentsText().append(displayProperties(getProperties()))
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
         ret.append("<p><b>")
            .append(variable->variableType().replace("<","&lt;"))
            .append(" ")
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
   QString returnType {_block->returnType()};
   if ( !returnType.isEmpty() && returnType != QString("void") )
   {
      ret.append("<h3>Return</h3><p><b>")
         .append(returnType.replace("<","&lt;"))
         .append(" :</b> ")
         .append(_block->returnDescription())
         .append("</p>");
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
            .append(parseBoldMarkers(list.at(i)))
            .append("</p>");
      }
   }
   return ret;
}






QStringList Function::getProperties()
{
   QStringList ret {Variable::getProperties()};
   if ( _block->isDefault() ) ret << "default";
   if ( _block->isExplicit() ) ret << "explicit";
   if ( _block->isVirtual() ) ret << "virtual";
   if ( _block->isConst() ) ret << "const";
   if ( _block->isNoExcept() ) ret << "noexcept";
   if ( _block->isOverride() ) ret << "override";
   if ( _block->isFinal() ) ret << "final";
   if ( _block->isAbstract() ) ret << "abstract(= 0)";
   return ret;
}
