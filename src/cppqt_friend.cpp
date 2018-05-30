#include "cppqt_friend.h"
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace CppQt;
//






/*!
 * Implements the interface that returns this block's type. 
 *
 * @return This block's type. 
 */
int Friend::type() const
{
   return BlockFactory::FriendType;
}






/*!
 * Implements the interface that returns the name of this block. 
 *
 * @return The name of this block. 
 */
QString Friend::name() const
{
   return "friend";
}






/*!
 * Implements the interface that returns a view that provides a detailed read only 
 * GUI representation of this block's data. 
 *
 * @return New GUI view that represents this block's data. 
 */
std::unique_ptr<QWidget> Friend::makeView() const
{
   return nullptr;
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Friend::makeEdit()
{
   return nullptr;
}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Friend::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Friend);
}






/*!
 *
 * @param value  
 */
void Friend::checkField(const QString& value)
{
   if ( !QRegExp("[a-zA-Z_]+[a-zA-Z_0-9<>\\*& ]*\\([a-zA-Z_0-9<>\\*&, ]*\\)").exactMatch(value) )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invalid friend field '%1'.").arg(value));
      throw e;
   }
}
