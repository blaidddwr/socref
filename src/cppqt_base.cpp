#include "cppqt_base.h"
#include "cppqt_blockfactory.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






Base::Base(const QString& name):
   _name(name)
{}






QString Base::name() const
{
   return _name;
}






const AbstractBlockFactory& Base::factory() const
{
   return BlockFactory::instance();
}






void Base::setName(const QString& name)
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
}






QString Base::description() const
{
   return _description;
}






void Base::setDescription(const QString& description)
{
   if ( _description != description )
   {
      _description = description;
      emit modified();
   }
}






void Base::readData(const QDomElement& data)
{
   enum
   {
      Description = 0
      ,Total
   };
   QStringList tags {"description"};
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
         }
      }
      node = node.nextSibling();
   }
}






QDomElement Base::writeData(QDomDocument& document) const
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
   return ret;
}






void Base::copyDataFrom(const Base& object)
{
   _name = object._name;
   _description = object._description;
}
