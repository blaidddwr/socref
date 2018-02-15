#include <QObject>
#include <exception.h>
#include "projectfactory.h"
#include "cppqt_blockfactory.h"
#include "cppqt_parserfactory.h"






int ProjectFactory::size() const
{
   return Total;
}






QString ProjectFactory::name(int type) const
{
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   default: return QString();
   }
}






QString ProjectFactory::defaultFilters(int type) const
{
   switch (type)
   {
   case CppQtType: return QString("*.cpp *.h");
   default: return QString();
   }
}






const AbstractBlockFactory& ProjectFactory::blockFactory(int type) const
{
   switch (type)
   {
   case CppQtType: return CppQt::BlockFactory::instance();
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot reference block factory of invalid type %1 when max is %2.").arg(type).arg(Total));
         throw e;
      }
   }
}






const AbstractParserFactory& ProjectFactory::parserFactory(int type) const
{
   switch (type)
   {
   case CppQtType: return CppQt::ParserFactory::instance();
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot reference parser factory of invalid type %1 when max is %2.").arg(type).arg(Total));
         throw e;
      }
   }
}
