#include "cppqt_base.h"
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace CppQt;
const char* Base::_nameTag {"name"};
const char* Base::_descriptionTag {"description"};






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






void Base::readData(const QDomElement& data, int version)
{
   switch (version)
   {
   case 0:
      readVersion0(data);
      break;
   case 1:
      readVersion1(data);
      break;
   default:
      {
         Exception::LogicError e;
         MARK_EXCEPTION(e);
         e.setDetails(tr("Unknown version number %1 given for reading block.").arg(version));
         throw e;
      }
   }
}






int Base::writeVersion() const
{
   return _version;
}






QDomElement Base::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   if ( !_name.isEmpty() ) ret.appendChild(makeElement(document,_nameTag,_name));
   if ( !_description.isEmpty() )
   {
      ret.appendChild(makeElement(document,_descriptionTag,_description));
   }
   return ret;
}






void Base::copyDataFrom(const AbstractBlock* object)
{
   if ( const Base* object_ = qobject_cast<const Base*>(object) )
   {
      _name = object_->_name;
      _description = object_->_description;
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails("Block object given to copy is not correct type");
      throw e;
   }
}






void Base::readVersion0(const QDomElement& data)
{
   DomElementReader reader(data);
   _name = reader.attribute(_nameTag,false);
   _description.clear();
   reader.set(_descriptionTag,&_description,false);
   reader.read();
}






void Base::readVersion1(const QDomElement& data)
{
   DomElementReader reader(data);
   reader.set(_nameTag,&_name,false);
   reader.set(_descriptionTag,&_description,false);
   reader.read();
}
