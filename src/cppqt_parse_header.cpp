#include <QStack>
#include "cppqt_parse_header.h"
#include "cppqt_parse_function.h"
#include "cppqt_function.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_class.h"
#include "cppqt_parse_common.h"
#include "cppqt_parse_variable.h"
#include "cppqt_parse_enumeration.h"



using namespace CppQt::Parse;






Header::Header(Namespace* block):
   Global(block),
   _block(block)
{
   buildDeclarations();
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
      if ( Function* child = findDefined(line) ) stepIntoChild(child);
      else
      {
         _undefined.append(new Function(line,this));
         stepIntoChild(_undefined.back());
      }
   }
   return true;
}






void Header::makeOutput()
{
   outputPreProcesser();
   outputHeader();
   outputDeclarations();
   outputDefinitions();
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






void Header::outputDeclarations()
{
   if ( !_declarations.isEmpty() )
   {
      addBlankLines(3);
      beginNamespaceNesting();
      Class* block;
      if ( ( block = qobject_cast<Class*>(_block) ) )
      {
         outputClassDeclaration(block);
         addLine("{");
         setIndent(indent() + 3);
      }
      for (auto declaration : qAsConst(_declarations)) declaration->outputDeclaration();
      if ( block )
      {
         setIndent(indent() - 3);
         addLine("};");
      }
   }
}






void Header::outputClassDeclaration(Class* block)
{
   QString line;
   QString templateString {getTemplateDeclaration(block)};
   if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
   line.append("class ");
   QString scope {getClassScope(block->parent())};
   if ( !scope.isEmpty() ) line.append(scope).append("::");
   line.append(block->Base::name());
   addLine(line);
}






void Header::outputDefinitions()
{
   for (auto function : _defined)
   {
      addBlankLines(6);
      function->outputComments();
      function->outputDefinition();
   }
   for (auto function : _undefined)
   {
      addBlankLines(6);
      function->outputComments();
      function->outputDefinition();
   }
   endNamespaceNesting();
}






Function* Header::findDefined(const QString& definition)
{
   for (auto function : qAsConst(_defined))
   {
      if ( function->isMatch(definition) ) return function;
   }
   return nullptr;
}






void Header::buildDeclarations()
{
   bool isTemplate {_block->hasAnyTemplates()};
   const QList<AbstractBlock*> list {_block->realChildren()};
   for (auto child : list)
   {
      if ( CppQt::Variable* valid = child->cast<CppQt::Variable>(BlockFactory::VariableType) ) _declarations.append(new Variable(valid,this));
      else if ( CppQt::Enumeration* valid = child->cast<CppQt::Enumeration>(BlockFactory::EnumerationType) ) _declarations.append(new Enumeration(valid,this));
      else if ( CppQt::Function* valid = qobject_cast<CppQt::Function*>(child) )
      {
         Function* base {new Function(valid,this)};
         _declarations.append(base);
         if ( isTemplate || valid->hasTemplates() ) _defined.append(base);
      }
   }
}
