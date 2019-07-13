#include "cppqt_accessview.h"
#include "cppqt_access.h"
#include "cppqt_factory.h"
#include "cppqt_function.h"



namespace CppQt
{






/*!
 * Constructs a new view widget with the given access block.
 *
 * @param block Access block this new view displays.
 */
AccessView::AccessView(const Access* block)
   :
   Basic::BlockView(block)
{
}






/*!
 * Returns rich text that displays the body of this view's access block. The
 * rich text returned reports the number of enumerations, variables, functions,
 * and classes this view's access block contains. If the count is zero this does
 * not display that block type.
 *
 * @return Rich text that displays the body of this view's access block.
 */
QString AccessView::displayInfo()
{
   // Initialize all counters.
   int enumerationAmt {0};
   int variableAmt {0};
   int functionAmt {0};
   int virtualAmt {0};
   int abstractAmt {0};
   int classAmt {0};
   int declareAmt {0};

   // Iterate through all block children of this view's access block.
   for (auto child : block<Access>().list())
   {
      // Increment the appropriate counter based off the child block's type.
      switch (child->type())
      {
      case Factory::Enumeration:
         enumerationAmt++;
         break;
      case Factory::Variable:
         variableAmt++;
         break;
      case Factory::Function:
         {
            // Figure out if this function block is abstract, virtual, or regular,
            // incrementing the correct counter based off that information.
            Function* valid {child->cast<Function>(Factory::Function)};
            if ( valid->isAbstract() ) abstractAmt++;
            else if ( valid->isVirtual() ) virtualAmt++;
            else functionAmt++;
            break;
         }
      case Factory::Class:
         classAmt++;
         break;
      case Factory::Declaration:
         declareAmt++;
         break;
      }
   }

   // Create an empty return string.
   QString ret;

   // Append rich text info about each block type if their count is above zero.
   if ( enumerationAmt ) ret += tr("%n enumeration(s)<br/>","",enumerationAmt);
   if ( variableAmt ) ret += tr("%n variable(s)<br/>","",variableAmt);
   if ( functionAmt ) ret += tr("%n function(s)<br/>","",functionAmt);
   if ( virtualAmt ) ret += tr("%n virtual function(s)<br/>","",virtualAmt);
   if ( abstractAmt ) ret += tr("%n abstract function(s)<br/>","",abstractAmt);
   if ( classAmt ) ret += tr("%n class(es)<br/>","",classAmt);
   if ( declareAmt ) ret += tr("%n declaration(s)<br/>","",declareAmt);

   // Return the rich text.
   return ret;
}

}
