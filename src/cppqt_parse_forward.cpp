#include "cppqt_parse_forward.h"
#include "cppqt_class.h"



using namespace CppQt::Parse;
//






/*!
 * Implements _CppQt::Parse::Base_ interface. This implementation does nothing 
 * because forward class declarations have no comments. 
 */
void Forward::outputComments()
{}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Forward::outputDeclaration()
{
   // Add the forward class declaration to output using this object's class. 
   add(QStringLiteral("class ") + _block->baseName() + QStringLiteral(";"));
}






/*!
 * Implements _CppQt::Parse::Base_ interface. This does nothing because forward 
 * class definitions do not exist. 
 */
void Forward::outputDefinition()
{}






/*!
 * Constructs a new forward parser with the given class block and parent parser. 
 *
 * @param block The class block that defines this new forward parser. 
 *
 * @param parent The parent parser for this new forward parser. 
 */
Forward::Forward(Class* block, AbstractParser* parent):
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
bool Forward::readLine(const QString& line)
{
   // Do nothing and return false. 
   Q_UNUSED(line)
   return false;
}
