#include "cppqt_parse_global.h"
#include <QStack>
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"



using namespace CppQt::Parse;






Global::Global(Namespace* block):
   _block(block)
{}






bool Global::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






void Global::makeOutput()
{
   add(1);
   beginNamespaceNesting();
   QList<Class*> list {_block->makeListOfType<Class>(BlockFactory::ClassType)};
   for (auto item : list)
   {
      add(QString("class ").append(item->Base::name()).append(";"));
   }
   endNamespaceNesting();
   add(1);
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
      setIndent(indent() + 3);
      ++_depth;
   }
}






void Global::endNamespaceNesting()
{
   while ( _depth > 0 )
   {
      setIndent(indent() - 3);
      --_depth;
      add("}");
   }
}
