#include <QObject>

#include "projectfactory.h"
#include "cppqt_blockfactory.h"
#include "exception.h"



//@@
namespace CppQt {

BlockFactory g_factory(ProjectFactory::CppQtType);

}






//@@
QString ProjectFactory::getName(int type) const
{
   // figure out which project type and return its name
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   default: return QString();
   }
}






//@@
QString ProjectFactory::getDefaultFilters(int type) const
{
   // figure out which project type and return its default filters
   switch (type)
   {
   case CppQtType: return QString("*.cpp *.h");
   default: return QString();
   }
}






//@@
const AbstractBlockFactory& ProjectFactory::getBlockFactory(int type) const
{
   // figure out which project type and return reference to its block factory
   switch (type)
   {
   case CppQtType: return CppQt::g_factory;
   default:
      // this is an unknown type so throw error
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot reference block factory of invalid type %1 when max is "
                                  "%2.").arg(type).arg(Total));
         throw e;
      }
   }
}
