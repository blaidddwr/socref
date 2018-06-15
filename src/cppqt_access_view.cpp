#include "cppqt_access_view.h"
#include "cppqt_blockfactory.h"



using namespace CppQt;
//






/*!
 * Constructs a new view object with the given variable block. 
 *
 * @param block Variable block this new view displays. 
 */
Access::View::View(const Access* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's variable block. 
 *
 * @return HTML rich text that displays the body of this view's variable block. 
 */
QString Access::View::displayText()
{
   //
   int functions {0};
   int variables {0};
   int signals_ {0};
   int slots_ {0};

   //
   for (auto child : _block->list())
   {
      //
      switch (child->type())
      {
      case BlockFactory::FunctionType:
         functions++;
         break;
      case BlockFactory::VariableType:
         variables++;
         break;
      case BlockFactory::SignalType:
         signals_++;
         break;
      case BlockFactory::SlotType:
         slots_++;
         break;
      }
   }

   //
   QString ret;

   //
   if ( _block->hasRegular() )
   {
      ret.append(QString::number(functions)).append(" functions<br/>");
      ret.append(QString::number(variables)).append(" variables<br/>");
   }

   //
   else if ( _block->hasSignals() ) ret.append(QString::number(signals_)).append(" signals<br/>");

   //
   else if ( _block->hasSlots() ) ret.append(QString::number(slots_)).append(" slots<br/>");

   //
   return ret;
}
