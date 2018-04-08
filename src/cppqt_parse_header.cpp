#include "cppqt_parse_header.h"
#include <QStack>
#include "cppqt_parse_function.h"
#include "cppqt_parse_common.h"
#include "cppqt_parse_variable.h"
#include "cppqt_parse_enumeration.h"
#include "cppqt_parse_access.h"
#include "cppqt_parse_forward.h"
#include "cppqt_parse_declaration.h"
#include "cppqt_function.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_class.h"
#include "cppqt_parent.h"
#include "cppqt_declaration.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;






Header::Header(Namespace* block, const QString& name):
   Source(block),
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces()),
   _headerLines(Settings::instance().headerLines())
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
   add(_headerLines);
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
   if ( !block->isDefault()
        && block->type() != BlockFactory::SignalType
        && ( isTemplate() || ( block->hasTemplates() && !block->isPrivateMethod() ) )
        && !block->isAbstract() )
   {
      addDefined(base);
   }
}






void Header::evaluateOther(AbstractBlock* block)
{
   if ( CppQt::Enumeration* valid = block->cast<CppQt::Enumeration>(BlockFactory::EnumerationType) )
   {
      _declarations.append(new Enumeration(valid,this));
   }
   if ( _block->type() == BlockFactory::ClassType )
   {
      if ( CppQt::Access* valid = block->cast<CppQt::Access>(BlockFactory::AccessType) )
      {
         _declarations.append(new Access(valid,this));
      }
      else if ( Class* valid = block->cast<Class>(BlockFactory::ClassType) )
      {
         _declarations.append(new Forward(valid,this));
      }
      else if ( CppQt::Declaration* valid = block->cast<CppQt::Declaration>(BlockFactory::DeclarationType) )
      {
         _declarations.append(new Declaration(valid,this));
      }
   }
}






void Header::outputHeader()
{
   add(_header1);
   add(_header2);
}






void Header::outputDeclarations()
{
   if ( !_declarations.isEmpty() )
   {
      Class* block;
      if ( ( block = qobject_cast<Class*>(_block) ) )
      {
         outputClassComments(block);
         outputClassDeclaration(block);
         add("{");
         setIndent(indent() + _indentSpaces);
         if ( block->isQtObject() ) add("Q_OBJECT");
      }
      for (auto declaration : qAsConst(_declarations)) declaration->outputDeclaration();
      if ( block )
      {
         setIndent(indent() - _indentSpaces);
         add("};");
      }
   }
}






void Header::outputClassComments(Class* block)
{
   add("/*!");
   add(makeComment(block->description()));
   if ( isTemplate() ) add(makeTemplateComments(_block));
   add(" */");
}






void Header::outputClassDeclaration(Class* block)
{
   QString line;
   QString templateString {getTemplateDeclaration(block)};
   if ( !templateString.isEmpty() ) line.append(templateString).append(" ");
   line.append("class ").append(getClassScope(block->parent())).append(block->Base::name());
   bool first {true};
   for (auto child : _block->list())
   {
      if ( Parent* valid = child->cast<Parent>(BlockFactory::ParentType) )
      {
         if ( first )
         {
            line.append(" : ");
            first = false;
         }
         else line.append(", ");
         line.append(valid->accessName()).append(" ").append(valid->className());
      }
   }
   add(line);
}






void Header::outputFooter()
{
   add(_headerLines);
   add("#endif");
}
