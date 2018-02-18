#include "cppqt_parse_commonheader.h"
#include "cppqt_parse_function.h"
#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "cppqt_function.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"



using namespace CppQt::Parse;






CommonHeader::CommonHeader(Namespace* common):
   _common(common)
{
   buildDefined();
}






bool CommonHeader::readLine(const QString& line)
{
   if ( QRegExp("#.*").exactMatch(line) ) _preprocesser << line;
   else if ( QRegExp("\\s*class\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*").exactMatch(line) ) _header << line.trimmed();
   else if ( QRegExp("\\s*[a-zA-Z0-9_,<>: ]*\\([a-zA-Z0-9_,<>: ]*\\):?\\s*").exactMatch(line) )
   {
      QString header {line.trimmed()};
      if ( Function* child = findDefined(header) ) stepIntoChild(child);
      else
      {
         _undefined.append(new Function(header,this));
         stepIntoChild(_undefined.back());
      }
   }
   return true;
}






void CommonHeader::makeOutput()
{
   outputPreProcesser();
   outputHeader();
   outputDeclarations();
   outputFunctions();
}






void CommonHeader::outputPreProcesser()
{
   for (auto line : _preprocesser) addLine(line);
}






void CommonHeader::outputHeader()
{
   if ( !_header.isEmpty() )
   {
      addBlankLines(3);
      for (auto line : _header) addLine(line);
   }
}






void CommonHeader::outputDeclarations()
{
   const QList<AbstractBlock*> list {_common->children()};
   for (auto child : list)
   {
      if ( child->type() == BlockFactory::VariableType ) outputVariable(qobject_cast<CppQt::Variable*>(child));
      else if ( child->type() == BlockFactory::EnumerationType ) outputEnumeration(qobject_cast<CppQt::Enumeration*>(child));
      else if ( CppQt::Function* func = qobject_cast<CppQt::Function*>(child) ) outputFunction(func);
   }
}






void CommonHeader::outputVariable(CppQt::Variable* block)
{
   if ( !block ) return;
   QString line;
   if ( !block->isClassMember() ) line.append("extern ");
   if ( block->isConstExpr() ) line.append("constexpr ");
   if ( block->isStatic() ) line.append("static ");
   line.append(block->variableType()).append(" ").append(block->Base::name());
   if ( block->hasInitializer() && block->isConstExpr() ) line.append("{").append(block->initializer()).append("}");
   line.append(";");
   addLine(line);
}






void CommonHeader::outputEnumeration(CppQt::Enumeration* block)
{
   if ( !block ) return;
   QString line {"enum"};
   if ( block->isClass() ) line.append(" class");
   addLine(line);
   addLine("{");
   setIndent(indent() + 3);
   const QList<EnumValue*> list {block->makeChildListOfType<EnumValue>(BlockFactory::EnumValueType)};
   bool first {true};
   for (auto value : list)
   {
      QString line;
      if ( first ) first = false;
      else line.append(",");
      line.append(value->Base::name());
      if ( value->hasValue() ) line.append(" = ").append(value->value());
      addLine(line);
   }
   setIndent(indent() - 3);
   addLine("}");
}






void CommonHeader::outputFunction(CppQt::Function* block)
{
   addLine(block->name().append(";"));
}






void CommonHeader::outputFunctions()
{
   for (auto func : _defined)
   {
      addBlankLines(6);
      func->makeOutput();
   }
   for (auto func : _undefined)
   {
      addBlankLines(6);
      func->makeOutput();
   }
}






Function* CommonHeader::findDefined(const QString& header)
{
   for (auto function : qAsConst(_defined))
   {
      if ( !function->hasCode() && function->header() == header ) return function;
   }
   return nullptr;
}






void CommonHeader::buildDefined()
{
   QList<CppQt::Function*> list {_common->makeChildListOfType<CppQt::Function>(BlockFactory::FunctionType)};
   for (auto function : list)
   {
      if ( function->hasTemplates() ) _defined.append(new Function(function,this));
   }
}
