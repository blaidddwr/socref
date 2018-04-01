#include "cppqt_namespace.h"
#include <memory>
#include <QDomDocument>
#include <exception.h>
#include "cppqt_view_namespace.h"
#include "cppqt_edit_namespace.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;






Namespace::Namespace(const QString& name):
   Base(name)
{}






int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






QIcon Namespace::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/namespace.svg");
   return ret;
}






QList<int> Namespace::buildList() const
{
   return QList<int>
   {
      BlockFactory::NamespaceType
      ,BlockFactory::VariableType
      ,BlockFactory::FunctionType
      ,BlockFactory::ClassType
      ,BlockFactory::OperatorType
      ,BlockFactory::EnumerationType
      ,BlockFactory::TypeListType
   };
}






unique_ptr<QWidget> Namespace::makeView() const
{
   return unique_ptr<QWidget>(new View::Namespace(this));
}






unique_ptr<AbstractEdit> Namespace::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Namespace(this));
}






QList<AbstractBlock*> Namespace::realChildren() const
{
   return list();
}






Namespace* Namespace::root()
{
   Namespace* ret {qobject_cast<Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }
   return ret;
}






const Namespace* Namespace::root() const
{
   const Namespace* ret {qobject_cast<const Namespace*>(AbstractBlock::root())};
   if ( !ret )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is not expected Namespace type."));
      throw e;
   }
   return ret;
}






unique_ptr<AbstractBlock> Namespace::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Namespace);
}
