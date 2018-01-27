#include <memory>
#include <QDomDocument>
#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "cppqt_gui_typedialog.h"
#include "domelementreader.h"



using namespace std;
using namespace CppQt;



const char* Namespace::_typeTag {"type"};
const char* Namespace::_nameTag {"name"};






Namespace::Namespace(const QString& name):
   Base(name)
{}






unique_ptr<AbstractBlock> Namespace::makeCopy() const
{
   unique_ptr<Namespace> ret {new Namespace};
   ret->copyChildren(this);
   ret->copyDataFrom(*this);
   ret->_types = _types;
   return ret;
}






int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






QStringList Namespace::types()
{
   return _types;
}






void Namespace::setTypes(const QStringList& types)
{
   QRegExp regexp(Gui::TypeDialog::_typeRegExp);
   for (auto type : types)
   {
      if ( !regexp.exactMatch(type) )
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Cannot set invalid type '%1'.").arg(type));
         throw e;
      }
   }
   _types = types;
   emit modified();
   emit typesChanged();
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






void Namespace::readData(const QDomElement& data)
{
   Base::readData(data);
   QList<QDomElement> types;
   DomElementReader reader(data);
   reader.set(_typeTag,&types,false);
   reader.read();
   for (auto type : qAsConst(types))
   {
      readType(type);
   }
}






QDomElement Namespace::writeData(QDomDocument& document) const
{
   QDomElement ret {Base::writeData(document)};
   for (auto typeName : qAsConst(_types))
   {
      QDomElement type {document.createElement(_typeTag)};
      type.setAttribute(_nameTag,typeName);
      ret.appendChild(type);
   }
   return ret;
}






void Namespace::readType(const QDomElement& type)
{
   DomElementReader reader(type);
   QString name {reader.attribute(_nameTag)};
   if ( _types.contains(name) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace type element found with duplicate entry."));
      throw e;
   }
   _types.append(name);
}
