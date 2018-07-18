#include "cppqt_friend.h"
#include <QRegularExpression>
#include <exception.h>
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Friend::type() const
{
   return BlockFactory::FriendType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Friend::name() const
{
   return "friend";
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Friend::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Friend);
}






/*!
 * Implements _CppQt::Using_ interface. 
 *
 * @param value See interface docs. 
 */
void Friend::checkField(const QString& value)
{
   // Make sure the given field value is a valid C++ function declaration for a 
   // friend declaration. 
   if ( !QRegularExpression("\\A[a-zA-Z_]+[a-zA-Z_0-9<>\\*& ]*\\([a-zA-Z_0-9<>\\*&, ]*\\)\\z").match(value).hasMatch() )
   {
      Exception::InvalidArgument e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("Invalid friend field '%1'.").arg(value));
      throw e;
   }
}
