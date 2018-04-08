#include "cppqt_parse_global.h"
#include <QStack>
#include "cppqt_parse_common.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;






Global::Global(Namespace* block):
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






bool Global::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






void Global::makeOutput()
{
   add(1);
   if ( _block->parent() )
   {
      add("/*!");
      add(makeComment(_block->description()));
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






void Global::beginNamespaceNesting()
{
   QStack<Namespace*> scope;
   AbstractBlock* block {_block};
   while ( block->parent() )
   {
      if ( Namespace* name = block->cast<Namespace>(BlockFactory::NamespaceType) ) scope.push(name);
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






void Global::endNamespaceNesting()
{
   while ( _depth > 0 )
   {
      setIndent(indent() - _indentSpaces);
      --_depth;
      add("}");
   }
}
