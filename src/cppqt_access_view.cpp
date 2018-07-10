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
   int functionAmt {0};
   int variableAmt {0};
   int signalAmt {0};
   int slotAmt {0};

   //
   for (auto child : _block->list())
   {
      //
      switch (child->type())
      {
      case BlockFactory::FunctionType:
         functionAmt++;
         break;
      case BlockFactory::VariableType:
         variableAmt++;
         break;
      case BlockFactory::SignalType:
         signalAmt++;
         break;
      case BlockFactory::SlotType:
         slotAmt++;
         break;
      }
   }

   //
   QString ret;

   //
   if ( _block->hasRegular() )
   {
      ret.append(tr("%n function(s)<br/>","0",functionAmt));
      ret.append(tr("%n variable(s)<br/>","0",variableAmt));
   }

   //
   else if ( _block->hasSignals() ) ret.append(tr("%n signal(s)<br/>","0",signalAmt));

   //
   else if ( _block->hasSlots() ) ret.append(tr("%n slot(s)<br/>","0",slotAmt));

   //
   return ret;
}
