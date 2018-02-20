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
   Source(block),
   _block(block)
{
   QString define {name.toUpper().append("_H")};
   _header1 = QString("#ifndef ").append(define);
   _header2 = QString("#define ").append(define);
}






void Header::makeOutput()
{
   outputHeader();
   outputPreProcesser();
   outputMisc();
   addBlankLines(3);
   beginNamespaceNesting();
   outputDeclarations();
   outputDefinitions();
   endNamespaceNesting();
   outputFooter();
}






void Header::readTop(const QString& line)
{
   if ( QRegExp("#.*").exactMatch(line) && line != _header1 && line != _header2 )
   {
      addPreProcess(line);
   }
   else if ( QRegExp("\\s*class\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*").exactMatch(line) )
   {
      addMisc(line.trimmed());
   }
}






void Header::evaluateVariable(CppQt::Variable* block)
{
   Variable* base {new Variable(block,this)};
   _declarations.append(base);
   if ( block->isStatic() && isTemplate() ) addVariable(base);
}






void Header::evaluateFunction(CppQt::Function* block)
{
   Function* base {new Function(block,this)};
   _declarations.append(base);
   if ( block->type() != BlockFactory::SignalType && ( isTemplate() || block->hasTemplates() ) && !block->isAbstract() )
   {
      addDefined(base);
   }
}






void Header::evaluateOther(AbstractBlock* block)
{
   if ( CppQt::Access* valid = block->cast<CppQt::Access>(BlockFactory::AccessType) )
   {
      _declarations.append(new Access(valid,this));
   }
   else if ( CppQt::Enumeration* valid = block->cast<CppQt::Enumeration>(BlockFactory::EnumerationType) )
   {
      _declarations.append(new Enumeration(valid,this));
   }
}






void Header::outputHeader()
{
   addLine(_header1);
   addLine(_header2);
}






void Header::outputDeclarations()
{
   if ( !_declarations.isEmpty() )
   {
      Class* block;
      if ( ( block = qobject_cast<Class*>(_block) ) )
      {
         if ( isTemplate() ) outputClassComments(block);
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
   addLine("/*!");
   addLines(makeComment(block->description()));
   addLines(makeTemplateComments(_block));
   addLine(" */");
}






void Header::outputClassDeclaration(Class* block)
{
   QString line;
   QString templateString {getTemplateDeclaration(block)};
   if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
   line.append("class ").append(getClassScope(block->parent())).append(block->Base::name());
   addLine(line);
}






void Header::outputFooter()
{
   addBlankLines(1);
   addLine("#endif");
   addBlankLines(1);
}
