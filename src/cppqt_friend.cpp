#include "cppqt_friend.h"
#include <QRegularExpression>
#include "cppqt_blockfactory.h"
#include "gui_abstractedit.h"



using namespace std;
using namespace CppQt;
//






/*!
 * Constructs a new friend block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Friend::Friend(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setField(QStringLiteral("class object"));
}






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
 * Implements _CppQt::Using_ interface. This implementation does nothing because 
 * the field of friend declarations can take many complex forms. 
 *
 * @param value See interface docs. 
 */
void Friend::checkField(const QString& value)
{
   Q_UNUSED(value)
}
