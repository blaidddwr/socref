#include "cppqt_signal.h"
#include "cppqt_signal_edit.h"
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
int Signal::type() const
{
   return BlockFactory::SignalType;
}






/*!
 * Implements the interface that returns the icon of this block. 
 *
 * @return The icon of this block. 
 */
QIcon Signal::icon() const
{
   static QIcon ret;
   if ( ret.isNull() ) ret = QIcon(":/icons/signal.svg");
   return ret;
}






/*!
 * Implements the interface that returns a editable GUI widget that provides the 
 * ability to edit this block's data. 
 *
 * @return New editable GUI widget to edit this block's data. 
 */
std::unique_ptr<::Gui::AbstractEdit> Signal::makeEdit()
{
   return unique_ptr<AbstractEdit>(new Edit(this));
}






/*!
 *
 * @param name  
 */
Signal::Signal(const QString& name):
   Slot(name)
{}






/*!
 * Implements the interface that makes a new block object of this block's type with 
 * no data and returns a pointer to the new block. 
 *
 * @return Pointer to the newly created block. 
 */
std::unique_ptr<AbstractBlock> Signal::makeBlank() const
{
   return unique_ptr<AbstractBlock>(new Signal);
}
