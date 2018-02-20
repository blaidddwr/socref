#include "cppqt_parse_enumeration.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_parse_common.h"



using namespace CppQt::Parse;






Enumeration::Enumeration(CppQt::Enumeration* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






void Enumeration::outputComments()
{}






void Enumeration::outputDeclaration()
{
   addLine("/*!");
   addLines(makeComment(_block->description()));
   addLine(" */");
   QString line {"enum "};
   if ( _block->isClass() ) line.append("class ");
   line.append(_block->Base::name());
   addLine(line);
   addLine("{");
   setIndent(indent() + 3);
   const QList<EnumValue*> list {_block->values()};
   bool first {true};
   for (auto value : list)
   {
      addLine("/*!");
      addLines(makeComment(value->description()));
      addLine(" */");
      QString line;
      if ( first ) first = false;
      else line.append(",");
      line.append(value->Base::name());
      if ( value->hasValue() ) line.append(" = ").append(QString::number(value->value()));
      addLine(line);
   }
   setIndent(indent() - 3);
   addLine("}");
}






void Enumeration::outputDefinition()
{}






bool Enumeration::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
