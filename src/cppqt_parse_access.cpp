#include "cppqt_parse_access.h"
#include "cppqt_access.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Implements _CppQt::Parse::Base_ interface. This implementation does nothing 
 * because access declarations have no comments. 
 */
void Access::outputComments()
{}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Access::outputDeclaration()
{
   // Decrease the indent by one, then add the access declaration line to output, and 
   // then increment the indent. 
   setIndent(indent() - _indentSpaces);
   add(_block->accessString());
   setIndent(indent() + _indentSpaces);
}






/*!
 * Implements _CppQt::Parse::Base_ interface. This does nothing because access 
 * definitions do not exist. 
 */
void Access::outputDefinition()
{}






/*!
 * Constructs a new access parser with the given access block and parent parser. 
 *
 * @param block The access block that defines this new access parser. 
 *
 * @param parent The parent parser of this new access parser. 
 */
Access::Access(CppQt::Access* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






/*!
 * Implements _AbstractParser_ interface. This interface does nothing because 
 * nothing unique is saved in their parsed files. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Access::readLine(const QString& line)
{
   // Do nothing and return false. 
   Q_UNUSED(line)
   return false;
}
