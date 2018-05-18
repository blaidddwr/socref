#include "cppqt_base.h"
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "domelementreader.h"
#include "common.h"



using namespace std;
using namespace CppQt;
//



/*!
 */
const char* Base::_nameTag {"name"};
/*!
 */
const char* Base::_descriptionTag {"description"};






/*!
 *
 * @param name  
 */
Base::Base(const QString& name):
   _name(name)
{}






/*!
 * Implements the interface that returns a reference to this block's factory. 
 *
 * @return Reference to block factory. 
 */
const AbstractBlockFactory& Base::factory() const
{
   return BlockFactory::instance();
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Base::name() const
{
   return _name;
}






/*!
 */
QString Base::description() const
{
   return _description;
}






/*!
 *
 * @param name  
 */
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
      notifyModified();
      notifyNameModified();
   }
}






/*!
 *
 * @param description  
 */
void Base::setDescription(const QString& description)
{
   if ( _description != description )
   {
      _description = description;
      notifyModified();
      notifyBodyModified();
   }
}






/*!
 * Implements the interface that reads in the data for this block from the given 
 * XML element and version number. 
 *
 * @param element The XML element used to read in this blocks data. 
 *
 * @param version The version of the data stored in the XML. 
 */
void Base::readData(const QDomElement& element, int version)
{
   switch (version)
   {
   case 0:
      readVersion0(element);
      break;
   case 1:
      readVersion1(element);
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






/*!
 * Implements the interface that returns the current version number of XML elements 
 * written for this block type. 
 *
 * @return Current version number. 
 */
int Base::writeVersion() const
{
   return _version;
}






/*!
 * Implements the interface that returns a XML element containing the data for this 
 * block using the current version number. 
 *
 * @param document XML document to use for creating new elements. 
 *
 * @return XML element containing the data of this block. 
 */
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






/*!
 * Implements the interface that copies all data from the given block to this 
 * block, overwriting any data this block may already contain. 
 *
 * @param other The other block whose data will be copied. 
 */
void Base::copyDataFrom(const AbstractBlock* other)
{
   if ( const Base* object_ = qobject_cast<const Base*>(other) )
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






/*!
 *
 * @param element The XML element used to read in this blocks data. 
 */
void Base::readVersion0(const QDomElement& element)
{
   DomElementReader reader(element);
   _name = reader.attribute(_nameTag,false);
   _description.clear();
   reader.set(_descriptionTag,&_description,false);
   reader.read();
}






/*!
 *
 * @param element The XML element used to read in this blocks data. 
 */
void Base::readVersion1(const QDomElement& element)
{
   DomElementReader reader(element);
   reader.set(_nameTag,&_name,false);
   reader.set(_descriptionTag,&_description,false);
   reader.read();
}
