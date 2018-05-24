#include "cppqt_slot.h"
#include "cppqt_view_slot.h"
#include "cppqt_edit_slot.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Slot::type() const
{
   return BlockFactory::SlotType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Slot::name() const
{
   return fullName(false,Base::name());
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Slot::icon() const
{
   static bool isLoaded {false};
   static QIcon regular;
   static QIcon virtual_;
   static QIcon abstract;
   if ( !isLoaded )
   {
      regular = QIcon(":/icons/slot.svg");
      virtual_ = QIcon(":/icons/vslot.svg");
      abstract = QIcon(":/icons/aslot.svg");
      isLoaded = true;
   }
   if ( isAbstract() ) return abstract;
   else if ( isVirtual() ) return virtual_;
   else return regular;
}






/*!
 * Implements the interface that returns a list of types that this block type can 
 * contain as children. 
 *
 * @return List of allowed types this block can contain as children. 
 */
QList<int> Slot::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Slot::makeView() const
{
   return unique_ptr<QWidget>(new View::Slot(this));
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Slot::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit::Slot(this));
}






/*!
 *
 * @param name  
 */
Slot::Slot(const QString& name):
   Function(name)
{}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Slot::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Slot);
}
