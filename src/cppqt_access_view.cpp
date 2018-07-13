#include "cppqt_access_view.h"
#include "cppqt_blockfactory.h"



using namespace CppQt;
//






/*!
 * Constructs a new view widget with the given access block. 
 *
 * @param block Access block this new view displays. 
 */
Access::View::View(const Access* block):
   _block(block)
{
   setText(displayText());
}






/*!
 * Returns the HTML rich text that displays the body of this view's access block. 
 * The HTML returned reports the number of functions, variables, signals, and slots 
 * this view's access block contains. 
 *
 * @return HTML rich text that displays the body of this view's access block. 
 */
QString Access::View::displayText()
{
   // Initialize all counters. 
   int functionAmt {0};
   int variableAmt {0};
   int signalAmt {0};
   int slotAmt {0};

   // Iterate through all block children of this view's access block. 
   for (auto child : _block->list())
   {
      // Increment the appropriate counter based off the child block's type. 
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

   // Create an empty return string. 
   QString ret;

   // If this view's access block has regular functions/variables then append those 
   // counters. 
   if ( _block->hasRegular() )
   {
      ret.append(tr("%n function(s)<br/>","0",functionAmt));
      ret.append(tr("%n variable(s)<br/>","0",variableAmt));
   }

   // Else if this view's access block has signals then append that counter. 
   else if ( _block->hasSignals() ) ret.append(tr("%n signal(s)<br/>","0",signalAmt));

   // Else if this view's access block has slots then append that counter. 
   else if ( _block->hasSlots() ) ret.append(tr("%n slot(s)<br/>","0",slotAmt));

   // Return the HTML body. 
   return ret;
}
