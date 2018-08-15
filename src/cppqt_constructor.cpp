#include "cppqt_constructor.h"
#include <socutil/sut_exceptions.h>
#include "cppqt_constructor_edit.h"
#include "cppqt_blockfactory.h"
#include "common.h"



using namespace Sut;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Constructor::type() const
{
   return BlockFactory::ConstructorType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Constructor::name() const
{
   // Return the full function name using no return type and the name of this 
   // constructor's class. 
   return fullName(false,className());
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Constructor::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/constructor.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Constructor::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<::Gui::AbstractEdit> Constructor::makeEdit()
{
   return QPtr<AbstractEdit>(new Edit(this));
}






/*!
 * Returns the name of this constructor's class. 
 *
 * @return Name of this constructor's class. 
 */
QString Constructor::className() const
{
   // Get the access block parent of this block and make sure it is not null or the 
   // root block. 
   AbstractBlock* access {parent()};
   if ( !access || !access->parent() )
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Parent or grandparent is nullptr."));
      throw e;
   }

   // Make sure the access block is a base block. 
   if ( Base* base = qobject_cast<Base*>(access->parent()) )
   {
      // Return the access block's base name. 
      return base->Base::name();
   }

   // Else the parent block is not a base block when it should be so throw an 
   // exception. 
   else
   {
      Exception::LogicError e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Grandparent does not contain CppQt::Base class."));
      throw e;
   }
}






/*!
 * Called when this constructor's class block has changed its name. This in turn 
 * notifies this block's name has changed. 
 */
void Constructor::classNameChanged()
{
   notifyNameModified();
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<AbstractBlock> Constructor::makeBlank() const
{
   return QPtr<AbstractBlock>(new Constructor);
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @param value See interface docs. 
 *
 * @return See interface docs. 
 */
bool Constructor::checkName(const QString& value)
{
   // Only accept empty strings as valid names. 
   return value.isEmpty();
}
