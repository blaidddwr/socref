#include "projectfactory.h"
#include <QObject>
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "cppqt_parserfactory.h"



using namespace std;
//






/*!
 */
int ProjectFactory::size() const
{
   return Total;
}






/*!
 *
 * @param type  
 */
QString ProjectFactory::name(int type) const
{
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   default: return QString();
   }
}






/*!
 *
 * @param type  
 */
QString ProjectFactory::defaultFilters(int type) const
{
   switch (type)
   {
   case CppQtType: return QString("*.cpp *.h");
   default: return QString();
   }
}






/*!
 *
 * @param type  
 */
const AbstractBlockFactory& ProjectFactory::blockFactory(int type) const
{
   switch (type)
   {
   case CppQtType: return CppQt::BlockFactory::instance();
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(
                  QObject::tr("Cannot reference block factory of invalid type %1 when max is %2.")
                  .arg(type)
                  .arg(Total));
         throw e;
      }
   }
}






/*!
 *
 * @param type  
 *
 * @param root  
 */
std::unique_ptr<AbstractParserFactory> ProjectFactory::makeParserFactory(int type, AbstractBlock* root) const
{
   switch (type)
   {
   case CppQtType: return unique_ptr<AbstractParserFactory>(new CppQt::ParserFactory(root));
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(
                  QObject::tr("Cannot reference parser factory of invalid type %1 when max is %2.")
                  .arg(type)
                  .arg(Total));
         throw e;
      }
   }
}
