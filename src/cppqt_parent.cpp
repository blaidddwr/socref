#include "cppqt_parent.h"
#include <QIcon>



namespace CppQt
{



/*!
 * List of access type names that follow the same order as this block's
 * enumeration of possible access types.
 */
const QStringList Parent::_accessNames {"public","protected","private"};






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QIcon Parent::icon() const
{
   // Initialize all static icons for this block type.
   static QIcon publicIcon(":/icons/parent.svg");
   static QIcon protectedIcon(":/icons/proparent.svg");
   static QIcon privateIcon(":/icons/priparent.svg");

   // Return the appropriate icon based off this parent block's access type, throwing
   // an exception if the access type is not recognized.
   switch (_accessNames.indexOf(accessString()))
   {
   case Access::Public: return publicIcon;
   case Access::Protected: return protectedIcon;
   case Access::Private: return privateIcon;
   default:
      Q_ASSERT(false);
   }
}






/*!
 * Return the string version of this block's access type.
 *
 * @return String version of this block's access type.
 */
QString Parent::accessString() const
{
   return getString("access");
}






/*!
 * Returns the class name of this parent block.
 *
 * @return Class name of this parent block.
 */
QString Parent::className() const
{
   return getString("class");
}

}
