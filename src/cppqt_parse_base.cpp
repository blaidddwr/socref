#include <QStack>
#include <exception.h>
#include "cppqt_parse_base.h"
#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_function.h"
#include "cppqt_operator.h"
#include "cppqt_class.h"
#include "cppqt_constructor.h"
#include "cppqt_destructor.h"
#include "cppqt_template.h"
#include "cppqt_common.h"



using namespace CppQt::Parse;






Base::Base(AbstractParser* parent):
   AbstractParser(parent)
{}






void Base::beginNamespaceNesting(AbstractBlock* block)
{
   QStack<Namespace*> scope;
   while ( block->parent() )
   {
      if ( Namespace* name = block->cast<Namespace>(BlockFactory::NamespaceType) ) scope.push(name);
      block = block->parent();
   }
   while ( !scope.isEmpty() )
   {
      addLine(QString("namespace ").append(scope.pop()->Base::name()));
      addLine("{");
      setIndent(indent() + 3);
      ++_nestDepth;
   }
}






void Base::endNamespaceNesting()
{
   while ( _nestDepth > 0 )
   {
      setIndent(indent() - 3);
      --_nestDepth;
      addLine("}");
   }
}






QString Base::variableDefinition(const CppQt::Variable* block, bool isHeader)
{
   if ( !block ) return QString();
   QString ret;
   if ( !isHeader )
   {
      if ( block->isClassMember() ) appendClass(block,&ret);
      else appendNamespace(block,&ret);
   }
   else
   {
      if ( !block->isClassMember() && !block->isConstExpr() && !block->isStatic() ) ret.append("extern ");
      if ( block->isConstExpr() ) ret.append("constexpr ");
      if ( block->isStatic() ) ret.append("static ");
   }
   ret.append(block->variableType()).append(" ").append(block->Base::name());
   if ( block->hasInitializer() )
   {
      if ( isHeader == block->isConstExpr() ) ret.append("{").append(block->initializer()).append("}");
   }
   ret.append(";");
   return ret;
}






QString Base::functionDefinition(const Function* block, bool withTemplate, bool withScope, bool withInitializers, bool allProperties)
{
   if ( !block ) return QString();
   QString ret;
   if ( withTemplate ) ret.append(templateName(block));
   if ( allProperties && block->isVirtual() ) ret.append("virtual ");
   if ( block->type() != BlockFactory::ConstructorType && block->type() != BlockFactory::DestructorType ) ret.append(block->returnType()).append(" ");
   if ( withScope )
   {
      if ( block->isMethod() )
      {
         appendClass(block,&ret,block->hasTemplates());
         ret.append("::");
      }
      else appendNamespace(block,&ret);
   }
   if ( const Operator* operator_ = qobject_cast<const Operator*>(block) ) ret.append("operator").append(operator_->operation());
   else if ( const Destructor* constructor = qobject_cast<const Destructor*>(block) ) ret.append("~").append(constructor->className());
   else if ( const Constructor* constructor = qobject_cast<const Constructor*>(block) ) ret.append(constructor->className());
   else ret.append(block->Base::name());
   appendArguments(block,&ret,withInitializers);
   if ( block->isConst() ) ret.append(" const");
   if ( block->isNoExcept() ) ret.append(" noexcept");
   if ( allProperties )
   {
      if ( block->isOverride() ) ret.append(" override");
      if ( block->isFinal() ) ret.append(" final");
      if ( block->isAbstract() ) ret.append(" = 0");
   }
   return ret;
}






void Base::appendClass(const AbstractBlock* block, QString* line, bool hasTemplate)
{
   QStack<const Class*> classes;
   while ( block )
   {
      if ( block->type() == BlockFactory::NamespaceType ) break;
      else if ( const Class* valid = qobject_cast<const Class*>(block) ) classes.push(valid);
      block = block->parent();
   }
   if ( classes.isEmpty() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Found no parents of type class when at least should be present."));
      throw e;
   }
   appendClassTemplate(classes.top(),line,true);
   while ( !classes.isEmpty() )
   {
      if ( classes.top()->hasTemplates() )
      {
         if ( hasTemplate )
         {
            Exception::LogicError e;
            MARK_EXCEPTION(e);
            e.setDetails(tr("Cannot parse object '%1' that has multiple nested templates.").arg(block->name()));
            throw e;
         }
         hasTemplate = true;
      }
      line->append(classes.top()->Base::name());
      appendClassTemplate(classes.pop(),line);
      if ( !classes.isEmpty() ) line->append("::");
   }
}






void Base::appendClassTemplate(const Class* block, QString* line, bool declare)
{
   if ( block->hasTemplates() )
   {
      const QList<Template*> list {block->templates()};
      bool first {true};
      if ( declare ) line->append("template");
      line->append("<");
      for (auto item : list)
      {
         if ( first ) first = false;
         else line->append(",");
         if ( declare ) line->append(item->name());
         else line->append(item->Base::name());
      }
      line->append(">");
      if ( declare ) line->append(" ");
   }
}






void Base::appendNamespace(const AbstractBlock* block, QString* line)
{
   Namespace* parent {qobject_cast<Namespace*>(block->parent())};
   if ( !parent )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent of function is type '%1' when it should be namespace.").arg(BlockFactory::instance().name(parent->type())));
      throw e;
   }
   if ( parent->parent() ) line->append(parent->Base::name()).append("::");
}






void Base::appendArguments(const Function* block, QString* line, bool withInitializers)
{
   line->append("(");
   const QList<Variable*> arguments {block->arguments()};
   bool first {true};
   for (auto argument : arguments)
   {
      if ( first ) first = false;
      else line->append(", ");
      line->append(argument->variableType()).append(" ").append(argument->Base::name());
      if ( withInitializers && argument->hasInitializer() ) line->append(" = ").append(argument->initializer());
   }
   line->append(")");
}






void Base::outputComment(const QString& text, int justified)
{
   if ( justified < 0 )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invalid justification of %1.").arg(justified));
      throw e;
   }
   if ( !text.isEmpty() )
   {
      bool first {true};
      QStringList words {text.split(QRegExp("\\s+"))};
      while ( !words.isEmpty() )
      {
         int total {indent() + 4 + words.first().size()};
         QString line {"/// "};
         if ( first ) first = false;
         else
         {
            for (int i = 0; i < justified ;++i) line.append(" ");
            total += justified;
         }
         line.append(words.takeFirst()).append(" ");
         while ( !words.isEmpty() && (total + words.first().size() + 1) <= 80 )
         {
            total += words.first().size() + 1;
            line.append(words.takeFirst()).append(" ");
         }
         addLine(line);
      }
   }
}
