#include "cppqt_constructor.h"
#include <exception.h>
#include "cppqt_constructor_edit.h"
#include "cppqt_blockfactory.h"
#include "common.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Constructor::type() const
{
   return BlockFactory::ConstructorType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Constructor::name() const
{
   return fullName(false,className());
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Constructor::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/constructor.svg");
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block type can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Constructor::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Constructor::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 */
QString Constructor::className() const
{
   AbstractBlock* access {parent()};
   if ( !access || !access->parent() )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Parent or grandparent is nullptr."));
      throw e;
   }
   if ( Base* base = qobject_cast<Base*>(access->parent()) )
   {
      return base->Base::name();
   }
   else
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Grandparent does not contain CppQt::Base class."));
      throw e;
   }
}






/*!
 */
void Constructor::classNameChanged()
{
   notifyNameModified();
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Constructor::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Constructor);
}






/*!
 * Implements _CppQt::Base_ interface. 
 *
 * @param value The name value whose syntax is checked to be valid or not. 
 *
 * @return See interface docs. 
 */
bool Constructor::checkName(const QString& value)
{
   return value.isEmpty();
}
