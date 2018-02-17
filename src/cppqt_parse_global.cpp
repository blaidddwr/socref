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
   int indent {0};
   addLine();
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
         addLine(QString("namespace ").append(name->Base::name()),indent);
         addLine("{",indent);
         indent += 3;
      }
   }
   QList<Class*> list {_root->makeChildListOfType<Class>(BlockFactory::ClassType)};
   for (auto item : list)
   {
      addLine(QString("class ").append(item->Base::name()).append(";"),indent);
   }
   while ( indent > 0 )
   {
      indent -= 3;
      addLine("}");
   }
   addLine();
}
