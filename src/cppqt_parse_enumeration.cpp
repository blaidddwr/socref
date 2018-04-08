#include "cppqt_parse_enumeration.h"
#include "cppqt_parse_common.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;






Enumeration::Enumeration(CppQt::Enumeration* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






void Enumeration::outputComments()
{}






void Enumeration::outputDeclaration()
{
   add("/*!");
   add(makeComment(_block->description()));
   add(" */");
   QString line {"enum "};
   if ( _block->isClass() ) line.append("class ");
   line.append(_block->Base::name());
   add(line);
   add("{");
   setIndent(indent() + _indentSpaces);
   const QList<EnumValue*> list {_block->values()};
   bool first {true};
   for (auto value : list)
   {
      add("/*!");
      add(makeComment(value->description()));
      add(" */");
      QString line;
      if ( first ) first = false;
      else line.append(",");
      line.append(value->Base::name());
      if ( value->hasValue() ) line.append(" = ").append(QString::number(value->value()));
      add(line);
   }
   setIndent(indent() - _indentSpaces);
   add("};");
}






void Enumeration::outputDefinition()
{}






bool Enumeration::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
