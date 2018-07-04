#include "cppqt_typelist.h"
#include "cppqt_typelist_view.h"
#include "cppqt_typelist_edit.h"
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int TypeList::type() const
{
   return BlockFactory::TypeListType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString TypeList::name() const
{
   return Base::name().append(" (").append(QString::number(size())).append(")");
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon TypeList::icon() const
{
   // If the static qt icon _ret_ is null then load its icon. 
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/typelist.svg");

   // Return _ret_. 
   return ret;
}






/*!
 * Implements the interface that returns a list of types that this block can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> TypeList::buildList() const
{
   return {BlockFactory::TypeListType,BlockFactory::TypeType};
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> TypeList::makeView() const
{
   return unique_ptr<QWidget>(new View(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> TypeList::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> TypeList::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new TypeList);
}






/*!
 * Implements the interface that is called whenever a child below this block has 
 * modified its name. 
 *
 * @param child Pointer to the child block that has modified its name. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool TypeList::childNameModified(AbstractBlock* child)
{
   Q_UNUSED(child)

   // Notify the body of this block is modified and return false. 
   notifyBodyModified();
   return false;
}






/*!
 * Implements the interface that is called whenever a new child below this block 
 * has been added. 
 *
 * @param child Pointer to the child block that been added to its new parent block. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool TypeList::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)

   // Notify the name and body of this block is modified and return false. 
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements the interface that is called whenever an existing child below this 
 * block has been removed. 
 *
 * @param child Pointer to the child block that has been removed from its former 
 *              parent block. This object can be deleted right after this call. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool TypeList::childRemoved(AbstractBlock* child)
{
   Q_UNUSED(child)

   // Notify the name and body of this block is modified and return false. 
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements the interface that checks to make sure the given name is a valid name 
 * for this block type, returning true if it is valid. 
 *
 * @param value The name value whose syntax is checked to be valid or not. 
 *
 * @return True if the given name is valid or false otherwise. 
 */
bool TypeList::checkName(const QString& value)
{
   return QRegExp("[a-zA-Z_]+((::)?[a-zA-Z0-9_])*").exactMatch(value);
}
