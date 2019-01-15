#include "glsl_parse_struct.h"
#include "glsl_factory.h"
#include "glsl_struct.h"
#include "glsl_variable.h"
#include "glsl_settings.h"



using namespace GLSL::Parse;
//






/*!
 * Implements _GLSL::Parse::Base_ interface. 
 */
void Struct::outputLines()
{
    add(Settings::instance().structLines());
}






/*!
 * Implements _GLSL::Parse::Base_ interface. 
 */
void Struct::outputDefinition()
{
    // .
    add(QStringLiteral("///"));
    addComment(_block->description());
    add(QStringLiteral("///"));

    // .
    add(makeLayout() + _block->typeString());
    add(QStringLiteral("{"));

    // .
    setIndent(indent() + 3);
    makeVariables();
    setIndent(indent() - 3);

    // .
    add(QStringLiteral("}"));
    add(_block->baseName() + QStringLiteral(";"));
}






/*!
 * Constructs a new struct parser with the given struct block and parent parser. 
 *
 * @param block Struct block that defines this new parser. 
 *
 * @param parent The parent parser object for this new variable parser. 
 */
Struct::Struct(const GLSL::Struct* block, AbstractParser* parent):
   Variable(block,parent),
   _block(block)
{}






/*!
 * Adds the definitions of all variables this parser object's struct block contains 
 * along with their description comment before every definition. 
 */
void Struct::makeVariables()
{
   // Get the list of all child variable blocks of this parser object's struct block. 
   const QList<GLSL::Variable*> list
   {
      _block->makeListOfType<GLSL::Variable>(GLSL::Factory::VariableType)
   };

   // Iterate through the list of variable blocks. 
   bool first {true};
   for (auto variable: list)
   {
      // If this is not the first variable definition then add one blank line. 
      if ( !first ) add(1);
      else first = false;

      // Add the description of the variable block to output. 
      add(QStringLiteral("///"));
      addComment(variable->description());
      add(QStringLiteral("///"));

      // Add the definition of the variable block to output. 
      add(variable->typeString()
          + QStringLiteral(" ")
          + variable->baseName()
          + QStringLiteral(";"));
   }
}
