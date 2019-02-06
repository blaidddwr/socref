#include "cppqt_parse_main.h"
#include "cppqt_parse_function.h"
#include "cppqt_namespace.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"



using namespace CppQt::Parse;
//






/*!
 * Constructs a new main parser with the given root namespace block. 
 *
 * @param block The root namespace block of the project used to find the special 
 *              main function. This must be the root namespace as the main function 
 *              is global. 
 */
Main::Main(const Namespace* block):
   Source(block)
{
   // Make sure the given namespace block is the root block for its project. 
   Q_ASSERT(block->parent() != nullptr);
}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Main::makeOutput()
{
   // Add any additional preprocessor lines to the output. 
   outputPreProcessor();

   // Add the header section to the output. 
   outputHeader();

   // Add the end of top separator. 
   add("//");

   // Add all function definitions to the output. 
   outputDefinitions();
}






/*!
 * Implements _CppQt::Parse::Source_ interface. This implementation does nothing 
 * because it is only interested in finding one special function block. 
 *
 * @param block See interface docs. 
 */
void Main::evaluateVariable(CppQt::Variable* block)
{
   Q_UNUSED(block)
}






/*!
 * Implements _CppQt::Parse::Source_ interface. 
 *
 * @param block See interface docs. 
 */
void Main::evaluateFunction(CppQt::Function* block)
{
   // Make sure the main function has not already been found. 
   if ( _found ) return;

   // Check to see if the given function block's name matches main. 
   if ( block->baseName() == QStringLiteral("main") )
   {
      // Check to see if the arguments for the given block matches the signature of 
      // main. 
      const QList<CppQt::Variable*> list {block->arguments()};
      if ( list.size() == 2
           && list.at(0)->variableType() == QStringLiteral("int")
           && list.at(1)->variableType() == QStringLiteral("char**") )
      {
         // This is the main function block so add it to this object's definitions and mark 
         // that the main function has been found. 
         addDefined(new Function(block,this));
         _found = true;
      }
   }
}






/*!
 * Implements _CppQt::Parse::Source_ interface. This implementation does nothing 
 * because it is only interested in finding one special function block. 
 *
 * @param block See interface docs. 
 */
void Main::evaluateOther(AbstractBlock* block)
{
   Q_UNUSED(block)
}
