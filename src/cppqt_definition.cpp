#include <memory>

#include "cppqt_definition.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






AbstractBlock* Definition::makeCopy() const
{
   unique_ptr<Definition> ret {new Definition(getFactory(),getType())};
   ret->_type = _type;
   return ret.release();
}






void Definition::setType(const Type& type)
{
   if ( !type.isBasic() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set definition type that is not basic."));
      throw e;
   }
   if ( _type != type )
   {
      _type = type;
      notifyOfNameChange();
      emit modified();
   }
}
