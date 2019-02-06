#include "cppqt_access.h"
#include <QRegularExpression>
#include "cppqt_access_view.h"
#include "cppqt_factory.h"
#include "cppqt_function.h"



using namespace CppQt;
//



/*!
 * List of access type names that follow the same order as this block's enumeration 
 * of possible types. 
 */
const QStringList Access::_typeNames
{
   "public:"
   ,"protected:"
   ,"private:"
   ,"signals:"
   ,"public slots:"
   ,"protected slots:"
   ,"private slots:"
};






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Access::name() const
{
   // Return the display name of this access block using its access type and the 
   // number of children it contains. 
   return QString("%1 (%2)").arg(accessString()).arg(size());
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Access::icon() const
{
   // Initialize all static icons for this block type. 
   static QIcon publicIcon(":/icons/public.svg");
   static QIcon signalsIcon(":/icons/signals.svg");
   static QIcon publicSlotsIcon(":/icons/pubslots.svg");
   static QIcon protectedIcon(":/icons/protected.svg");
   static QIcon protectedSlotsIcon(":/icons/proslots.svg");
   static QIcon privateIcon(":/icons/private.svg");
   static QIcon privateSlotsIcon(":/icons/prislots.svg");

   // Based off this access block's type return the appropriate icon. 
   switch (_typeNames.indexOf(getString("type")))
   {
   case Type::Public: return publicIcon;
   case Type::Signals: return signalsIcon;
   case Type::PublicSlots: return publicSlotsIcon;
   case Type::Protected: return protectedIcon;
   case Type::ProtectedSlots: return protectedSlotsIcon;
   case Type::Private: return privateIcon;
   case Type::PrivateSlots: return privateSlotsIcon;
   default:
      Q_ASSERT(false);
   }
}






/*!
 * Tests of this access block is a private type, returning true if it is. 
 *
 * @return True if this access block is a private type or false otherwise. 
 */
bool Access::isPrivate() const
{
   return QRegularExpression("\\Aprivate").match(accessString()).hasMatch();
}






/*!
 * Tests of this access block is a signal type, returning true if it is. 
 *
 * @return True if this access block is a signal type or false otherwise. 
 */
bool Access::isSignal() const
{
   return accessString() == _typeNames.at(Type::Signals);
}






/*!
 * Tests of this access block is a slot type, returning true if it is. 
 *
 * @return True if this access block is a slot type or false otherwise. 
 */
bool Access::isSlot() const
{
   return QRegularExpression("slots:\\z").match(accessString()).hasMatch();
}






/*!
 * Tests if this access block contains any function block that is virtual. 
 *
 * @return True if this access block contains any function block that is virtual or 
 *         false otherwise. 
 */
bool Access::hasVirtual() const
{
   // Iterate through all this block's children. 
   for (auto child : list())
   {
      // If the child block is a function and that function is virtual then return true. 
      if ( Function* valid = child->cast<Function>(Factory::FunctionType) )
      {
         if ( valid->isVirtual() ) return true;
      }
   }

   // No virtual functions was found so return false. 
   return false;
}






/*!
 * Tests if this access block contains any function block that is abstract. 
 *
 * @return True if this access block contains any function block that is abstract 
 *         or false otherwise. 
 */
bool Access::hasAbstract() const
{
   // Iterate through all this block's children. 
   for (auto child : list())
   {
      // If the child block is a function and that function is abstract then return 
      // true. 
      if ( Function* valid = child->cast<Function>(Factory::FunctionType) )
      {
         if ( valid->isAbstract() ) return true;
      }
   }

   // No abstract function was found so return false. 
   return false;
}






/*!
 * Returns this block's access type as a string. 
 *
 * @return This block's access type a a string. 
 */
QString Access::accessString() const
{
   return getString("type");
}






/*!
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<BasicBlock::View> Access::makeBasicView() const
{
   return new View(this);
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::childNameModified(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Return true to pass this event up to its parent class block. 
   return true;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::childAdded(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body of this block has changed. 
   notifyNameModified();
   notifyBodyModified();

   // Return true to pass this event up to its parent class block. 
   return true;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @param child See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::childRemoved(AbstractBlock* child)
{
   // This does not use the given child pointer. 
   Q_UNUSED(child)

   // Notify the name and body of this block has changed. 
   notifyNameModified();
   notifyBodyModified();

   // Return true to pass this event up to its parent class block. 
   return true;
}
