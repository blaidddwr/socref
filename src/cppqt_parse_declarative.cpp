#include "cppqt_parse_declarative.h"
#include "cppqt_declaration.h"



using namespace CppQt::Parse;
//






/*!
 * Implements _CppQt::Parse::Base_ interface. This does nothing because declarative 
 * comments do not exist. 
 */
void Declarative::outputComments()
{}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Declarative::outputDeclaration()
{
   // Add the single declarative statement of the using or friend block of this 
   // parser to output. 
   add(_block->line() + QStringLiteral(";"));
}






/*!
 * Implements _CppQt::Parse::Base_ interface. This does nothing because declarative 
 * definitions do not exist. 
 */
void Declarative::outputDefinition()
{}






/*!
 * Constructs a new declarative parser with the given using or friend block and 
 * parent parser. 
 *
 * @param block The declaration block that defines this new declarative parser. 
 *
 * @param parent The parent parser for this new declarative parser. 
 */
Declarative::Declarative(const CppQt::Declaration* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






/*!
 * Implements _AbstractParser_ interface. This interface does nothing because 
 * nothing unique is saved in their parsed files. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Declarative::readLine(const QString& line)
{
   Q_UNUSED(line)

   // Do nothing and return false. 
   return false;
}
