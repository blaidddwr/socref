#include "projectfactory.h"
#include <QObject>
#include "abstract_block.h"
#include "cppqt_factory.h"
#include "cppqt_settingsdialog.h"
#include "cppqt_parse_common.h"
#include "glsl_factory.h"
#include "glsl_settingsdialog.h"



//






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @return See interface docs. 
 */
int ProjectFactory::size() const
{
   return Total;
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString ProjectFactory::name(int type) const
{
   // Return the display name of the given project type. 
   switch (type)
   {
   case CppQtType: return QObject::tr("C++/Qt");
   case GLSLType: return QObject::tr("GLSL");
   default:
      // This project type is not recognized so throw an exception. 
      Q_ASSERT(false);
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param elementName See interface docs. 
 *
 * @return See interface docs. 
 */
int ProjectFactory::typeByElementName(const QString& elementName) const
{
   // Initialize a static list of element names for all project types that must match 
   // the same order of enumerated project types in this factory. 
   static const QStringList names {"cppqt","glsl"};

   // Return the project type of the given name that matches the index in the static 
   // list of names, returning -1 if no match is found. 
   return names.indexOf(elementName);
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString ProjectFactory::elementName(int type) const
{
   // Return the element name of the given project type. 
   switch (type)
   {
   case CppQtType: return QStringLiteral("cppqt");
   case GLSLType: return QStringLiteral("glsl");
   default:
      // This project type is not recognized so throw an exception. 
      Q_ASSERT(false);
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString ProjectFactory::defaultFilters(int type) const
{
   // Return default file filters for the given project type. 
   switch (type)
   {
   case CppQtType: return QStringLiteral("*.cpp *.h");
   case GLSLType: return QStringLiteral("*.glsl");
   default:
      // This project type is not recognized so throw an exception. 
      Q_ASSERT(false);
   }
}






/*!
 * Implements _AbstractProjectFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<QDialog> ProjectFactory::makeSettings(int type) const
{
   // Return a new settings dialog for the given project type. 
   switch (type)
   {
   case CppQtType: return Soc::Ut::QPtr<QDialog>(new CppQt::SettingsDialog);
   case GLSLType: return Soc::Ut::QPtr<QDialog>(new GLSL::SettingsDialog);
   default:
      // This project type is not recognized so throw an exception. 
      Q_ASSERT(false);
   }
}






/*!
 * Implements _Abstract::ProjectFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
const Abstract::BlockFactory& ProjectFactory::blockFactory(int type) const
{
   // Return a reference to the block factory of the given project type. 
   switch (type)
   {
   case CppQtType: return CppQt::Factory::instance();
   case GLSLType: return GLSL::Factory::instance();
   default:
      // This project type is not recognized so throw an exception. 
      Q_ASSERT(false);
   }
}






/*!
 * Implements _Abstract::ProjectFactory_ interface. 
 *
 * @param root See interface docs. 
 *
 * @return See interface docs. 
 */
QMap<QString,Scanner*> ProjectFactory::createScannerMap(const Abstract::Block* root) const
{
   // Figure out the project type of the given root block and then create and return 
   // a mapping of scanner objects, making sure the project type is recognized. 
   switch (root->factory().type())
   {
   case CppQtType: return CppQt::Parse::createScannerMap(root);
   //case GLSLType: return GLSL::Factory::instance();
   default:
      Q_ASSERT(false);
   }
}
