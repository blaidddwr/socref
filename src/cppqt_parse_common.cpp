#include "cppqt_parse_common.h"
#include <QStack>
#include <exception.h>
#include "cppqt_template.h"
#include "cppqt_blockfactory.h"
#include "cppqt_class.h"
#include "cppqt_namespace.h"
#include "cppqt_template.h"
#include "cppqt_settings.h"



using namespace CppQt;






QStringList Parse::makeTemplateComments(const AbstractBlock* block)
{
   QStringList ret;
   for (auto child : block->makeListOfType<Template>(BlockFactory::TemplateType))
   {
      ret << QString(" *");
      QString line {"@tparam "};
      line.append(child->Base::name()).append(" ");
      int justified {line.size()};
      line.append(child->description());
      ret << makeComment(line,justified);
   }
   return ret;
}






QStringList Parse::makeComment(const QString& text, int justified)
{
   if ( justified < 0 )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Invalid justification of %1.").arg(justified));
      throw e;
   }
   Settings& settings {Settings::instance()};
   QStringList ret;
   if ( !text.isEmpty() )
   {
      bool first {true};
      QStringList words {text.split(QRegExp("\\s+"))};
      while ( !words.isEmpty() )
      {
         int total {words.first().size()};
         QString line {" * "};
         if ( first ) first = false;
         else
         {
            for (int i = 0; i < justified ;++i) line.append(" ");
            total += justified;
         }
         line.append(words.takeFirst()).append(" ");
         while ( !words.isEmpty() && (total + words.first().size() + 1) <= settings.maxColumns() )
         {
            total += words.first().size() + 1;
            line.append(words.takeFirst()).append(" ");
         }
         ret << line;
      }
   }
   return ret;
}






QString Parse::getNamespace(const AbstractBlock* block)
{
   QString ret;
   Namespace* parent {qobject_cast<Namespace*>(block->parent())};
   if ( !parent )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Parent of function is type '%1' when it should be namespace.")
                   .arg(BlockFactory::instance().name(parent->type())));
      throw e;
   }
   if ( parent->parent() ) ret = parent->Base::name().append("::");
   return ret;
}






QString Parse::getClassScope(const AbstractBlock* block)
{
   QString ret;
   QStack<const Class*> classes;
   while ( block->parent() )
   {
      if ( block->type() == BlockFactory::NamespaceType ) break;
      else if ( const Class* valid = qobject_cast<const Class*>(block) ) classes.push(valid);
      block = block->parent();
   }
   if ( !classes.isEmpty() )
   {
      while ( !classes.isEmpty() )
      {
         ret.append(getClass(classes.pop())).append("::");
      }
   }
   return ret;
}






QString Parse::getClass(const Class* block)
{
   QString ret {block->Base::name()};
   ret.append(getTemplates(block));
   return ret;
}






QString Parse::getTemplateDeclaration(const AbstractBlock* block)
{
   while ( block->parent() )
   {
      if ( block->type() == BlockFactory::NamespaceType ) break;
      else if ( block->containsType(BlockFactory::TemplateType) ) return getTemplates(block,true);
      block = block->parent();
   }
   return QString();
}






QString Parse::getTemplates(const AbstractBlock* block, bool declare)
{
   QString ret;
   const QList<Template*> list {block->makeListOfType<Template>(BlockFactory::TemplateType)};
   if ( !list.isEmpty() )
   {
      bool first {true};
      if ( declare ) ret.append("template");
      ret.append("<");
      for (auto item : list)
      {
         if ( first ) first = false;
         else ret.append(",");
         if ( declare ) ret.append(item->variableType()).append(" ").append(item->name());
         else ret.append(item->Base::name());
      }
      ret.append(">");
   }
   return ret;
}
