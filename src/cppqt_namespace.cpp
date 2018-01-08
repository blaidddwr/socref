#include <memory>

#include <QDomDocument>

#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
#include "xmlelementparser.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






unique_ptr<AbstractBlock> Namespace::makeCopy() const
{
   unique_ptr<Namespace> ret {new Namespace};
   ret->_name = _name;
   ret->_description = _description;
   ret->copyChildren(this);
   return ret;
}






int Namespace::type() const
{
   return BlockFactory::NamespaceType;
}






const AbstractBlockFactory&Namespace::factory() const
{
   return BlockFactory::instance();
}






Namespace& Namespace::setName(const QString& name)
{
   if ( !QRegExp("[a-zA-Z_]*[a-zA-Z0-9_]*").exactMatch(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set invalid namespace '%1'.").arg(name));
      throw e;
   }
   if ( _name != name )
   {
      _name = name;
      notifyOfNameChange();
      emit modified();
   }
   return *this;
}






Namespace& CppQt::Namespace::setDescription(const QString& description)
{
   if ( _description != description )
   {
      _description = description;
      emit modified();
   }
   return *this;
}






void Namespace::readData(const QDomElement& data)
{
   if ( !data.hasAttribute("name") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace block missing name attribute."));
      throw e;
   }
   _name = data.attribute("name");
   _description = data.text();
}






QDomElement Namespace::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.setAttribute("name",_name);
   ret.appendChild(document.createTextNode(_description));
   return ret;
}
