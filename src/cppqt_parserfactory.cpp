#include "cppqt_parserfactory.h"
#include <exception.h>
#include "cppqt_parse_global.h"
#include "cppqt_parse_header.h"
#include "cppqt_parse_source.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_blockfactory.h"
#include "abstractblock.h"



using namespace std;
using namespace CppQt;






ParserFactory::ParserFactory(AbstractBlock* root):
   _root(qobject_cast<Namespace*>(root))
{
   if ( !_root )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(
               tr("Root block is type '%1' intead of namespace.")
               .arg(root->factory().name(root->type())));
      throw e;
   }
}






std::unique_ptr<AbstractParser> ParserFactory::make(const QString& name, const QString& extension) const
{
   if ( name == QString("global") && extension == QString("h") )
   {
      return unique_ptr<AbstractParser>(new Parse::Global(_root));
   }
   else if ( !name.isEmpty() )
   {
      bool isCommon {false};
      QStringList names {name.split('_')};
      if ( names.back() == QString("common") )
      {
         names.takeLast();
         isCommon = true;
      }
      if ( extension == QString("h") )
      {
         return unique_ptr<AbstractParser>(find(_root,names,name,true,isCommon));
      }
      else if ( extension == QString("cpp") )
      {
         return unique_ptr<AbstractParser>(find(_root,names,name,false,isCommon));
      }
   }
   return nullptr;
}





AbstractParser* ParserFactory::find(Namespace* current, const QStringList& names, const QString& name, bool isHeader, bool isCommon, int index) const
{
   if ( names.isEmpty() )
   {
      if ( isCommon )
      {
         if ( isHeader ) return new Parse::Header(current,name);
         else return new Parse::Source(current,name);
      }
      else return nullptr;
   }
   QList<AbstractBlock*> list {current->realChildren()};
   for (auto child : list)
   {
      if ( Namespace* next = qobject_cast<Namespace*>(child) )
      {
         if ( next->Base::name().toLower() == names.at(index) )
         {
            if ( ++index == names.size() )
            {
               if ( !isCommon && next->type() == BlockFactory::NamespaceType )
               {
                  return new Parse::Global(next);
               }
               else
               {
                  if ( isHeader ) return new Parse::Header(next,name);
                  else return new Parse::Source(next,name);
               }
            }
            else return find(next,names,name,isHeader,isCommon,index);
         }
      }
   }
   return nullptr;
}
