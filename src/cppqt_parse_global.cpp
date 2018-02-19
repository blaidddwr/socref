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
   beginNamespaceNesting(_root);
   QList<Class*> list {_root->makeChildListOfType<Class>(BlockFactory::ClassType)};
   for (auto item : list)
   {
      addLine(QString("class ").append(item->Base::name()).append(";"));
   }
   endNamespaceNesting();
   addBlankLines(1);
}
