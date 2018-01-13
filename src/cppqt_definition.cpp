#include "cppqt_definition.h"
#include "cppqt_blockfactory.h"
#include "cppqt_type_factory.h"
#include "cppqt_type_concrete.h"



using namespace std;
using namespace CppQt;






Definition::Definition():
   _type(new Type::Concrete)
{}






Definition::Definition(const QString& name):
   _type(new Type::Concrete(name))
{}






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
   return BlockFactory::DefinitionType;
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
   _type = Type::Factory::instance().read(data);
}






QDomElement Definition::writeData(QDomDocument& document) const
{
   return _type->write(document);
}
