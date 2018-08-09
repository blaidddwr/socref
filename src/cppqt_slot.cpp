#include "cppqt_slot.h"
#include "cppqt_slot_edit.h"
#include "cppqt_blockfactory.h"



using namespace std;
using namespace Gui;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
int Slot::type() const
{
   return BlockFactory::SlotType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Slot::name() const
{
   // Return the full function name without any return type and with the base name 
   // field. 
   return fullName(false,Base::name());
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Slot::icon() const
{
   // Initialize all static icons for this block type. 
   static QIcon regularIcon(":/icons/slot.svg");
   static QIcon virtualIcon(":/icons/vslot.svg");
   static QIcon abstractIcon(":/icons/aslot.svg");

   // Return the appropriate icon based off the properties of this slot block. 
   if ( isAbstract() ) return abstractIcon;
   else if ( isVirtual() ) return virtualIcon;
   else return regularIcon;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QList<int> Slot::buildList() const
{
   return QList<int>{BlockFactory::VariableType};
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Slot::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new slot block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Slot::Slot(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setName(QStringLiteral("slot"));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Slot::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Slot);
}
