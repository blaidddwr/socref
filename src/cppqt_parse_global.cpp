#include "cppqt_parse_global.h"
#include <QStack>
#include "cppqt_parse_base.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 *
 * @param block  
 */
Global::Global(const Namespace* block):
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






/*!
 *
 * @param line  
 */
bool Global::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






/*!
 */
void Global::makeOutput()
{
   add(1);
   if ( _block->parent() )
   {
      add("/*!");
      add(Base::makeComment(_block->description()));
      add(" */");
   }
   beginNamespaceNesting();
   QList<Class*> list {_block->makeListOfType<Class>(BlockFactory::ClassType)};
   for (auto item : list)
   {
      if ( !item->hasTemplates() ) add(QString("class ").append(item->Base::name()).append(";"));
   }
   endNamespaceNesting();
}






/*!
 */
void Global::beginNamespaceNesting()
{
   QStack<const Namespace*> scope;
   const AbstractBlock* block {_block};
   while ( block->parent() )
   {
      if ( const Namespace* name = block->cast<Namespace>(BlockFactory::NamespaceType) )
      {
         scope.push(name);
      }
      block = block->parent();
   }
   while ( !scope.isEmpty() )
   {
      add(QString("namespace ").append(scope.pop()->Base::name()));
      add("{");
      setIndent(indent() + _indentSpaces);
      ++_depth;
   }
}






/*!
 */
void Global::endNamespaceNesting()
{
   while ( _depth > 0 )
   {
      setIndent(indent() - _indentSpaces);
      --_depth;
      add("}");
   }
}
