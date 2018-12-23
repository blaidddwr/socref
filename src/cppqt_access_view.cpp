#include "cppqt_access_view.h"
#include "cppqt_factory.h"



using namespace CppQt;
//






/*!
 * Constructs a new view widget with the given access block. 
 *
 * @param block Access block this new view displays. 
 */
Access::View::View(const Access* block):
   BasicBlock::View(block),
   _block(block)
{}






/*!
 * Returns rich text that displays the body of this view's access block. The rich 
 * text returned reports the number of enumerations, variables, functions, and 
 * classes this view's access block contains. If the count is zero this does not 
 * display that block type. 
 *
 * @return Rich text that displays the body of this view's access block. 
 */
QString Access::View::displayInfo()
{
   // Initialize all counters. 
   int enumerationAmt {0};
   int variableAmt {0};
   int functionAmt {0};
   int classAmt {0};
   int declareAmt {0};

   // Iterate through all block children of this view's access block. 
   for (auto child : _block->list())
   {
      // Increment the appropriate counter based off the child block's type. 
      switch (child->type())
      {
      case Factory::EnumerationType:
         enumerationAmt++;
         break;
      case Factory::VariableType:
         variableAmt++;
         break;
      case Factory::FunctionType:
         functionAmt++;
         break;
      case Factory::ClassType:
         classAmt++;
         break;
      case Factory::DeclarationType:
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
   if ( classAmt ) ret += tr("%n class(es)<br/>","",classAmt);
   if ( declareAmt ) ret += tr("%n declaration(s)<br/>","",declareAmt);

   // Return the rich text. 
   return ret;
}
