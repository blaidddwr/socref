#include "glsl_parse_variable.h"
#include "glsl_variable.h"



using namespace GLSL::Parse;
//






/*!
 * Constructs a new variable parser with the given variable block and parent 
 * parser. 
 *
 * @param block Variable block that defines this new parser. 
 *
 * @param parent The parent parser object for this new variable parser. 
 */
Variable::Variable(const GLSL::Variable* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






/*!
 * Implements _AbstractParser_ interface. This parser does not read in any input so 
 * this does nothing. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Variable::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}






/*!
 * Implements _AbstractParser_ interface. This outputs the definition of this 
 * parser object's variable block. 
 */
void Variable::makeOutput()
{
   // Output the description of this parser object's variable block. 
   add(QStringLiteral("///"));
   addComment(_block->description());
   add(QStringLiteral("///"));

   // Output the definition. 
   add(makeLayout()
       + _block->typeString()
       + QStringLiteral(" ")
       + _block->baseName()
       + QStringLiteral(";"));
}






/*!
 * Returns the layout part of this parser object's variable definition. 
 *
 * @return Layout part of this parser object's variable definition. 
 */
QString Variable::makeLayout()
{
   // Create a new string that will be returned and get all layout qualifiers of this 
   // parser object's variable block. 
   QString ret;

   // If there are no qualifiers then return an empty string. 
   const QStringList qualifiers {_block->layout()};
   if ( qualifiers.isEmpty() ) return ret;

   // Add the beginning of the layout definition. 
   ret += QStringLiteral("layout (");

   // Iterate through all qualifiers and add them to the layout definition. 
   bool first {true};
   for (auto qualifier: qualifiers)
   {
      if ( !first ) ret += QStringLiteral(", ");
      else first = false;
      ret += qualifier;
   }

   // Add the end of the layout definition and return it. 
   ret += QStringLiteral(") ");
   return ret;
}
