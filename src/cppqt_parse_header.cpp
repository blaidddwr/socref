#include <QStack>
#include "cppqt_parse_header.h"
#include "cppqt_parse_function.h"
#include "cppqt_function.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_class.h"



using namespace CppQt::Parse;






Header::Header(Namespace* block):
   _block(block)
{
   buildDefined();
}






bool Header::readLine(const QString& line)
{
   if ( line.trimmed() == QString("{") ) _pastHeader = true;
   if ( !_pastHeader )
   {
      if ( QRegExp("#.*").exactMatch(line) ) _preprocesser << line;
      else if ( QRegExp("\\s*class\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*").exactMatch(line) ) _header << line.trimmed();
   }
   if ( QRegExp(".*\\([a-zA-Z0-9_,<>: ]*\\):?\\s*").exactMatch(line) )
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






void Header::makeOutput()
{
   outputPreProcesser();
   outputHeader();
   addBlankLines(3);
   beginNamespaceNesting(_block);
   Class* block;
   if ( ( block = qobject_cast<Class*>(_block) ) )
   {
      outputClassDefinition(block);
      addLine("{");
      setIndent(indent() + 3);
   }
   outputDeclarations(_block);
   if ( block )
   {
      setIndent(indent() - 3);
      addLine("};");
   }
   outputFunctions();
   endNamespaceNesting();
   addBlankLines(1);
}






void Header::outputPreProcesser()
{
   for (auto line : _preprocesser) addLine(line);
}






void Header::outputHeader()
{
   if ( !_header.isEmpty() )
   {
      addBlankLines(3);
      for (auto line : _header) addLine(line);
   }
}






void Header::outputClassDefinition(Class* block)
{
   QString line {"class "};
   appendClass(block,&line,false);
   addLine(line);
}






void Header::outputDeclarations(AbstractBlock* block)
{
   if ( Access* access = qobject_cast<Access*>(block) )
   {
      setIndent(indent() - 3);
      addLine(Access::_typeNames.at(static_cast<int>(access->accessType())));
      setIndent(indent() + 3);
   }
   const QList<AbstractBlock*> list {block->children()};
   for (auto child : list)
   {
      if ( block->type() == BlockFactory::ClassType )
      {
         if ( child->type() == BlockFactory::AccessType ) outputDeclarations(child);
      }
      else if ( child->type() == BlockFactory::VariableType ) outputVariable(qobject_cast<CppQt::Variable*>(child));
      else if ( child->type() == BlockFactory::EnumerationType ) outputEnumeration(qobject_cast<CppQt::Enumeration*>(child));
      else if ( CppQt::Function* func = qobject_cast<CppQt::Function*>(child) ) outputFunction(func);
   }
}






void Header::outputFunctions()
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






void Header::outputVariable(const CppQt::Variable* block)
{
   addLine(variableDefinition(block,true));
}






void Header::outputFunction(const CppQt::Function* block)
{
   QString line {Base::functionDefinition(block,true,false,true,true)};
   if ( !line.isEmpty() ) addLine(line.append(";"));
}






void Header::outputEnumeration(const Enumeration* block)
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






Function* Header::findDefined(const QString& definition)
{
   for (auto function : qAsConst(_defined))
   {
      if ( function->isMatch(definition) ) return function;
   }
   return nullptr;
}






void Header::buildDefined()
{
   bool isTemplate {_block->hasAnyTemplates()};
   QList<AbstractBlock*> list {_block->realChildren()};
   for (auto child : list)
   {
      if ( CppQt::Function* func = qobject_cast<CppQt::Function*>(child) )
      {
         if ( isTemplate || func->hasTemplates() ) _defined.append(new Function(func,this));
      }
   }
}
