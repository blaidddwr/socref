#include "projectfactory.h"
#include <QObject>
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "cppqt_parserfactory.h"
#include "cppqt_gui_settingsdialog.h"



using namespace std;
//






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @return Explained in interface. 
 */
int ProjectFactory::size() const
{
   return Total;
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type Explained in interface. 
 *
 * @return Explained in interface. 
 */
QString ProjectFactory::name(int type) const
{
   // Return the display name of the given project type. 
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   default:
      {
         // This project type is not recognized so throw an exception. 
         Exception::InvalidArgument e;
         e.setDetails(QObject::tr("Unrecognized project type %1.").arg(type));
         throw e;
      }
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type Explained in interface. 
 *
 * @return Explained in interface. 
 */
QString ProjectFactory::defaultFilters(int type) const
{
   // Return default file filters for the given project type. 
   switch (type)
   {
   case CppQtType: return QString("*.cpp *.h");
   default:
      {
         // This project type is not recognized so throw an exception. 
         Exception::InvalidArgument e;
         e.setDetails(QObject::tr("Unrecognized project type %1.").arg(type));
         throw e;
      }
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type Explained in interface. 
 *
 * @return Explained in interface. 
 */
std::unique_ptr<QDialog> ProjectFactory::makeSettings(int type) const
{
   // Return a new settings dialog for the given project type. 
   switch (type)
   {
   case CppQtType: return unique_ptr<QDialog>(new CppQt::Gui::SettingsDialog);
   default:
      {
         // This project type is not recognized so throw an exception. 
         Exception::InvalidArgument e;
         e.setDetails(QObject::tr("Unrecognized project type %1.").arg(type));
         throw e;
      }
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type Explained in interface. 
 *
 * @return Explained in interface. 
 */
const AbstractBlockFactory& ProjectFactory::blockFactory(int type) const
{
   // Return a reference to the block factory of the given project type. 
   switch (type)
   {
   case CppQtType: return CppQt::BlockFactory::instance();
   default:
      {
         // This project type is not recognized so throw an exception. 
         Exception::InvalidArgument e;
         e.setDetails(QObject::tr("Unrecognized project type %1.").arg(type));
         throw e;
      }
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type Explained in interface. 
 *
 * @param root Explained in interface. 
 *
 * @return Explained in interface. 
 */
std::unique_ptr<AbstractParserFactory> ProjectFactory::makeParserFactory(int type, AbstractBlock* root) const
{
   // Create a new parser factory for the given project type using the given block 
   // root. 
   switch (type)
   {
   case CppQtType: return unique_ptr<AbstractParserFactory>(new CppQt::ParserFactory(root));
   default:
      {
         // This project type is not recognized so throw an exception. 
         Exception::InvalidArgument e;
         e.setDetails(QObject::tr("Unrecognized project type %1.").arg(type));
         throw e;
      }
   }
}
