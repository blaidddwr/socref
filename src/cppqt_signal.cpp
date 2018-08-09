#include "cppqt_signal.h"
#include "cppqt_signal_edit.h"
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
int Signal::type() const
{
   return BlockFactory::SignalType;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QIcon Signal::icon() const
{
   // Initialize the static icon for this block type. 
   static QIcon ret(":/icons/signal.svg");

   // Return the icon. 
   return ret;
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<::Gui::AbstractEdit> Signal::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 * Constructs a new signal block with a default state or null state based off the 
 * given flag. 
 *
 * @param isDefault True to initialize this new block to its default state or false 
 *                  to leave it in a null state. 
 */
Signal::Signal(bool isDefault)
{
   // If the given flag is set to default then initialize this new block. 
   if ( isDefault ) setName(QStringLiteral("signal"));
}






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
std::unique_ptr<AbstractBlock> Signal::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Signal);
}
