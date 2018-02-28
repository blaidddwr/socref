#include "cppqt_parse_variable.h"
#include "cppqt_variable.h"
#include "cppqt_parse_common.h"



using namespace CppQt::Parse;






Variable::Variable(CppQt::Variable* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






void Variable::outputComments()
{
   addLine("/*!");
   addLines(makeComment(_block->description()));
   addLine(" */");
}






void Variable::outputDeclaration()
{
   outputComments();
   QString line;
   if ( _block->isConstExpr() ) line.append("constexpr ");
   if ( _block->isStatic() ) line.append("static ");
   if ( line.isEmpty() && !_block->isClassMember() ) line.append("extern ");
   line.append(_block->variableType()).append(" ").append(_block->Base::name());
   if ( _block->hasInitializer() && _block->isConstExpr() )
   {
      line.append(" {").append(_block->initializer()).append("}");
   }
   line.append(";");
   addLine(line);
}






void Variable::outputDefinition()
{
   if ( _block->isStatic() || !_block->isClassMember() )
   {
      QString line;
      QString templateString {getTemplateDeclaration(_block)};
      if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
      line.append(_block->variableType()).append(" ");
      if ( _block->isClassMember() ) line.append(getClassScope(_block));
      else line.append(getNamespace(_block));
      line.append(_block->Base::name());
      if ( _block->hasInitializer() && !_block->isConstExpr() )
      {
         line.append(" {").append(_block->initializer()).append("}");
      }
      line.append(";");
      addLine(line);
   }
}






bool Variable::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
