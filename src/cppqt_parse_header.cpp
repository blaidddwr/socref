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
#include "cppqt_parse_access.h"



using namespace CppQt::Parse;






Header::Header(Namespace* block, const QString& name):
   Global(block),
   _block(block),
   _isTemplate(block->hasAnyTemplates())
{
   QString define {name.toUpper().append("_H")};
   _header1 = QString("#ifndef ").append(define);
   _header2 = QString("#define ").append(define);
   buildDeclarations();
}






bool Header::readLine(const QString& line)
{
   if ( line.trimmed() == QString("{") ) _pastHeader = true;
   if ( !_pastHeader )
   {
      if ( QRegExp("#.*").exactMatch(line) && line != _header1 && line != _header2 )
      {
         _preprocesser << line;
      }
      else if ( QRegExp("\\s*class\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*").exactMatch(line) )
      {
         _misc << line.trimmed();
      }
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
   outputHeader();
   outputPreProcesser();
   outputMisc();
   outputDeclarations();
   outputDefinitions();
   outputFooter();
}






void Header::outputHeader()
{
   addLine(_header1);
   addLine(_header2);
}






void Header::outputPreProcesser()
{
   for (auto line : _preprocesser) addLine(line);
}






void Header::outputMisc()
{
   if ( !_misc.isEmpty() )
   {
      addBlankLines(3);
      for (auto line : _misc) addLine(line);
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
         if ( _isTemplate ) outputClassComments(block);
         outputClassDeclaration(block);
         addLine("{");
         setIndent(indent() + 3);
         if ( block->isQtObject() ) addLine("Q_OBJECT");
      }
      for (auto declaration : qAsConst(_declarations)) declaration->outputDeclaration();
      if ( block )
      {
         setIndent(indent() - 3);
         addLine("};");
      }
   }
}






void Header::outputClassComments(Class* block)
{
   addLines(makeComment(block->description()));
   addLines(makeTemplateComments(_block));
   addLine("///");
}






void Header::outputClassDeclaration(Class* block)
{
   QString line;
   QString templateString {getTemplateDeclaration(block)};
   if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
   line.append("class ").append(getClassScope(block->parent())).append(block->Base::name());
   addLine(line);
}






void Header::outputDefinitions()
{
   if ( !_variables.isEmpty() )
   {
      addBlankLines(3);
      for (auto variable : qAsConst(_variables)) variable->outputDefinition();
   }
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






void Header::outputFooter()
{
   addBlankLines(1);
   addLine("#endif");
   addBlankLines(1);
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
   const QList<AbstractBlock*> list {_block->realChildren()};
   for (auto child : list)
   {
      if ( CppQt::Access* valid = child->cast<CppQt::Access>(BlockFactory::AccessType) )
      {
         _declarations.append(new Access(valid,this));
      }
      else if ( CppQt::Enumeration* valid = child->cast<CppQt::Enumeration>(BlockFactory::EnumerationType) )
      {
         _declarations.append(new Enumeration(valid,this));
      }
      else if ( CppQt::Variable* valid = child->cast<CppQt::Variable>(BlockFactory::VariableType) )
      {
         Variable* base {new Variable(valid,this)};
         _declarations.append(base);
         if ( valid->isStatic() && _isTemplate ) _variables.append(base);
      }
      else if ( CppQt::Function* valid = qobject_cast<CppQt::Function*>(child) )
      {
         Function* base {new Function(valid,this)};
         _declarations.append(base);
         if ( valid->type() != BlockFactory::SignalType && ( _isTemplate || valid->hasTemplates() ) )
         {
            _defined.append(base);
         }
      }
   }
}
