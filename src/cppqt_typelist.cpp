#include "cppqt_typelist.h"
#include "cppqt_view_typelist.h"
#include "cppqt_edit_typelist.h"
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
   QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/typelist.svg");
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
   QList<int> ret {BlockFactory::TypeListType,BlockFactory::TypeType};
   return ret;
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> TypeList::makeView() const
{
   return unique_ptr<QWidget>(new View::TypeList(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> TypeList::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::TypeList(this));
}






/*!
 *
 * @param name  
 */
TypeList::TypeList(const QString& name):
   Base(name)
{}






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
 * modified its name and keeps calling this interface on the next block parent 
 * until this returns false. 
 *
 * @param child Pointer to the child block that has modified its name. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool TypeList::childNameModified(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements the interface that is called whenever a new child below this block 
 * has been added and keeps calling this interface on the next block parent until 
 * this returns false. 
 *
 * @param child Pointer to the child block that been added to its new parent block. 
 *
 * @return True if this interface should be called again on this blocks parent or 
 *         false otherwise. 
 */
bool TypeList::childAdded(AbstractBlock* child)
{
   Q_UNUSED(child)
   notifyNameModified();
   notifyBodyModified();
   return false;
}






/*!
 * Implements the interface that is called whenever an existing child below this 
 * block has been removed and keeps calling this interface on the next block parent 
 * until this returns false. 
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
   notifyNameModified();
   notifyBodyModified();
   return false;
}
