#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomElement>

#include "cppqt_definition.h"
#include "cppqt_blockfactory.h"
#include "exception.h"
#include "xmlelementparser.h"
#include "cppqt_typefactory.h"
#include "cppqt_abstracttype.h"



using namespace std;
using namespace CppQt;






QString Definition::name() const
{
   return _type->fullName();
}






unique_ptr<AbstractBlock> Definition::makeCopy() const
{
   unique_ptr<Definition> ret {new Definition};
   ret->_type = _type->makeCopy();
   return ret;
}






int Definition::type() const
{
   return -1;
}






const AbstractBlockFactory&Definition::factory() const
{
   return BlockFactory::instance();
}






Definition& Definition::setType(unique_ptr<AbstractType>&& type)
{
   if ( *type != *_type )
   {
      _type = std::move(type);
      notifyOfNameChange();
      emit modified();
   }
   return *this;
}






void Definition::readData(const QDomElement& data)
{
   _type = TypeFactory::instance().read(data);
}






QDomElement Definition::writeData(QDomDocument& document) const
{
   return _type->write(document);
}
