#include <memory>
#include <QDomDocument>
#include "cppqt_namespace.h"
#include "cppqt_blockfactory.h"
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






int Namespace::type() const { return BlockFactory::NamespaceType; }






const AbstractBlockFactory& Namespace::factory() const { return BlockFactory::instance(); }






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






QStringList Namespace::types() { return _types; }






Namespace& Namespace::setTypes(const QStringList& types)
{
   //TODO check each type for correctness
   _types = types;
   return *this;
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
   enum
   {
      Description = 0
      ,Type
      ,Total
   };
   QStringList tags {"description","type"};
   if ( data.hasAttribute("name") )
   {
      _name = data.attribute("name");
   }
   QDomNode node {data.firstChild()};
   while ( !node.isNull() )
   {
      if ( node.isElement() )
      {
         QDomElement element {node.toElement()};
         switch (tags.indexOf(element.tagName()))
         {
         case Description:
            _description = element.text();
            break;
         case Type:
            readType(element);
            break;
         }
      }
      node = node.nextSibling();
   }
}






QDomElement Namespace::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   if ( !_name.isEmpty() )
   {
      ret.setAttribute("name",_name);
   }
   if ( !_description.isEmpty() )
   {
      QDomElement description {document.createElement("description")};
      description.appendChild(document.createTextNode(_description));
      ret.appendChild(description);
   }
   for (auto typeName : qAsConst(_types))
   {
      typeName.replace("<","(");
      typeName.replace(">",")");
      QDomElement type {document.createElement("type")};
      type.setAttribute("name",typeName);
      ret.appendChild(type);
   }
   return ret;
}






void Namespace::readType(const QDomElement &type)
{
   if ( !type.hasAttribute("name") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace type element missing name attribute."));
      throw e;
   }
   if ( _types.contains(type.attribute("name")) )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("C++/Qt Namespace type element found with duplicate entry."));
      throw e;
   }
   QString name {type.attribute("name")};
   name.replace("(","<");
   name.replace(")",">");
   _types.append(name);
}
