#include "cppqt_parse_variable.h"
#include "cppqt_parse_common.h"
#include "cppqt_variable.h"



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
   outputEnd(&line, _block->hasInitializer()
             && ( _block->isConstExpr() || ( _block->isClassMember() && !_block->isStatic() ) ));
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
      outputEnd(&line, _block->hasInitializer() && !_block->isConstExpr() );
   }
}






bool Variable::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






void Variable::outputEnd(QString* line, bool withInitializer)
{
   if ( withInitializer )
   {
      const QString initialize {_block->initializer()};
      if ( (initialize.size() + line->size() + indent() + 1) > 100 )
      {
         addLine(*line);
         addLine("{");
         setIndent(indent() + 3);
         bool first {true};
         const QStringList parts {initialize.split(',')};
         for (auto part : parts)
         {
            QString line;
            if ( first ) first = false;
            else line.append(",");
            line.append(part);
            addLine(line);
         }
         setIndent(indent() - 3);
         addLine("};");
      }
      else
      {
         line->append(" {").append(initialize).append("};");
         addLine(*line);
      }
   }
   else
   {
      line->append(";");
      addLine(*line);
   }
}
