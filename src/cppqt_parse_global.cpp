#include <QStack>
#include "cppqt_parse_global.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"



using namespace CppQt::Parse;






Global::Global(Namespace* root):
   _root(root)
{}






bool Global::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






void Global::makeOutput()
{
   addBlankLines(1);
   QStack<AbstractBlock*> scope;
   AbstractBlock* parent {_root->parent()};
   while ( parent )
   {
      scope.push(parent);
      parent = parent->parent();
   }
   while ( !scope.isEmpty() )
   {
      if ( Namespace* name = qobject_cast<Namespace*>(scope.pop()) )
      {
         addLine(QString("namespace ").append(name->Base::name()));
         addLine("{");
         setIndent(indent() + 3);
      }
   }
   QList<Class*> list {_root->makeChildListOfType<Class>(BlockFactory::ClassType)};
   for (auto item : list)
   {
      addLine(QString("class ").append(item->Base::name()).append(";"));
   }
   while ( indent() > 0 )
   {
      setIndent(indent() - 3);
      addLine("}");
   }
   addBlankLines(1);
}
