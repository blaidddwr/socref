#include "projectfactory.h"
#include <QObject>
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "cppqt_parserfactory.h"
#include "cppqt_gui_settingsdialog.h"



using namespace std;
//






/*!
 * This implements the interface that returns the number of project types that 
 * exist for the program. 
 *
 * @return Number of project types that exist. 
 */
int ProjectFactory::size() const
{
   return Total;
}






/*!
 * This implements the interface that returns the display name for the given 
 * project type. 
 *
 * @param type Project type whose display name is returned. 
 *
 * @return Display name for the given project type. 
 */
QString ProjectFactory::name(int type) const
{
   // Return the display name of the given project type. If the given type does not 
   // exist then return an empty string. 
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   default: return QString();
   }
}






/*!
 * This implements the interface that returns the default file filters for the 
 * given project type. 
 *
 * @param type Project type whose default file filters are returned. 
 *
 * @return Default file filters for the given project type. 
 */
QString ProjectFactory::defaultFilters(int type) const
{
   // Return default file filters for the given project type. If the given type does 
   // not exist then return an empty string. 
   switch (type)
   {
   case CppQtType: return QString("*.cpp *.h");
   default: return QString();
   }
}






/*!
 * This implements the interface that makes a new settings dialog for the given 
 * project type. 
 *
 * @param type Project type whose settings dialog is returned. 
 *
 * @return Pointer to the new settings dialog for the given project type. 
 */
std::unique_ptr<QDialog> ProjectFactory::makeSettings(int type) const
{
   // Return a new settings dialog for the given project type. If the given type does 
   // not exist then throw an exception. 
   switch (type)
   {
   case CppQtType: return unique_ptr<QDialog>(new CppQt::Gui::SettingsDialog);
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot create settings dialog of invalid type %1 when max is %2.")
                      .arg(type)
                      .arg(Total - 1));
         throw e;
      }
   }
}






/*!
 * This implements the interface that returns a read only reference to the block 
 * factory for the given project type. 
 *
 * @param type Project type whose block factory is returned as a reference. 
 *
 * @return Read only reference to block factory for the given project type. 
 */
const AbstractBlockFactory& ProjectFactory::blockFactory(int type) const
{
   // Return a reference to the block factory of the given project type. If the given 
   // project type does not exist then throw an exception. 
   switch (type)
   {
   case CppQtType: return CppQt::BlockFactory::instance();
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot reference block factory of invalid type %1 when max is %2.")
                      .arg(type)
                      .arg(Total - 1));
         throw e;
      }
   }
}






/*!
 * This implements the interface that makes a new parser factory for the given 
 * project type. 
 *
 * @param type Project type whose parser factory type is made and returned. 
 *
 * @param root Pointer to root block of existing project that is used by returned 
 *             parser object. 
 *
 * @return Pointer to created parser factory for the given project type. 
 */
std::unique_ptr<AbstractParserFactory> ProjectFactory::makeParserFactory(int type, AbstractBlock* root) const
{
   // Create a new parser factory for the given project type using the given block 
   // root. If the given project type does not exist then throw an exception. 
   switch (type)
   {
   case CppQtType: return unique_ptr<AbstractParserFactory>(new CppQt::ParserFactory(root));
   default:
      {
         Exception::InvalidArgument e;
         MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Cannot create parser factory of invalid type %1 when max is %2.")
                      .arg(type)
                      .arg(Total - 1));
         throw e;
      }
   }
}
