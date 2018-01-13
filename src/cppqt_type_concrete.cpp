#include <QDomElement>
#include "cppqt_type_concrete.h"
#include "exception.h"
#include "cppqt_type_factory.h"



using namespace CppQt;
using namespace CppQt::Type;
using namespace std;






bool Concrete::isEquivalent(const AbstractType* type) const
{
   if ( const Concrete* type_ = dynamic_cast<const Concrete*>(type) )
   {
      return name() == type_->name();
   }
   return false;
}






AbstractType* Concrete::read(const QDomElement& type)
{
   if ( !type.hasAttribute("name") )
   {
      Exception::ReadError e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("C++/Qt concrete type missing name attribute."));
      throw e;
   }
   setName(type.attribute("name"));
   return this;
}






int Concrete::type() const
{
   return Type::Factory::ConcreteType;
}






QDomElement Concrete::writeData(QDomDocument& document) const
{
   QDomElement ret {document.createElement("na")};
   ret.setAttribute("name",name());
   return ret;
}
