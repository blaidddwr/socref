#include <exception.h>
#include "cppqt_parserfactory.h"
#include "cppqt_parse_global.h"
#include "cppqt_parse_commonheader.h"
#include "abstractblock.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace CppQt;






ParserFactory::ParserFactory(AbstractBlock* root):
   _root(qobject_cast<Namespace*>(root))
{
   if ( !_root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Root block is type '%1' intead of namespace.").arg(root->factory().name(root->type())));
      throw e;
   }
}






std::unique_ptr<AbstractParser> ParserFactory::makeParser(const QString& name, const QString& extension) const
{
   if ( extension == QString("h") )
   {
      if ( name == QString("global") ) return unique_ptr<AbstractParser>(new Parse::Global(_root));
      else
      {
         QStringList names {name.split('_')};
         if ( names.back() == QString("common") )
         {
            names.takeLast();
            Namespace* item;
            if ( names.isEmpty() ) item = _root;
            else item = find(_root,&names);
            if ( item && item->type() == BlockFactory::NamespaceType ) return unique_ptr<AbstractParser>(new Parse::CommonHeader(item));
         }
         else if ( Namespace* item = find(_root,&names) )
         {
            if ( item->type() == BlockFactory::NamespaceType ) return unique_ptr<AbstractParser>(new Parse::Global(item));
         }
      }
   }
   return nullptr;
}






Namespace* ParserFactory::find(const Namespace* current, QStringList* names) const
{
   if ( names->isEmpty() ) return nullptr;
   const QList<AbstractBlock*> list {current->children()};
   for (auto child : list)
   {
      if ( Namespace* next = qobject_cast<Namespace*>(child) )
      {
         if ( next->Base::name() == names->first() )
         {
            names->takeFirst();
            if ( names->isEmpty() )
            {
               return next;
            }
            else return find(next,names);
         }
      }
   }
   return nullptr;
}
