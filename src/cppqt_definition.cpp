#include <memory>

#include "cppqt_definition.h"
#include "exception.h"



using namespace std;
using namespace CppQt;






//@@
AbstractBlock* Definition::makeCopy() const
{
   // create new definition
   unique_ptr<Definition> ret {new Definition(getFactory(),getType())};

   // copy data to new definition and return its pointer
   ret->_type = _type;
   return ret.release();
}






//@@
void Definition::setType(const Type& type)
{
   // make sure the type is basic
   if ( !type.isBasic() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set definition type that is not basic."));
      throw e;
   }

   // check if new type is different from current
   if ( _type != type )
   {
      // set new type, notify of name change, and emit modified signal
      _type = type;
      notifyOfNameChange();
      emit modified();
   }
}
