#include "cppqt_type_concrete.h"
#include "exception.h"



using namespace CppQt;
using namespace CppQt::Type;
using namespace std;






bool Concrete::isEquivalent(const AbstractType* type) const
{
   if ( const Concrete* type_ = dynamic_cast<const Concrete*>(type) )
   {
      return _name == type_->_name;
   }
   return false;
}






unique_ptr<AbstractType> Concrete::makeCopy() const
{
   unique_ptr<Concrete> ret {new Concrete()};
   ret->_name = _name;
   return ret;
}






Concrete& Concrete::setName(const QString& name)
{
   if ( !QRegExp("[a-zA-Z_]+[a-zA-Z0-9_]*").exactMatch(name) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(QObject::tr("Cannot set concrete type name to '%1'.").arg(name));
      throw e;
   }
   _name = name;
   return *this;
}
