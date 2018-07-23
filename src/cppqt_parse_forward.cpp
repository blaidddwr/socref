#include "cppqt_parse_forward.h"
#include "cppqt_class.h"



using namespace CppQt::Parse;
//






/*!
 */
void Forward::outputComments()
{}






/*!
 */
void Forward::outputDeclaration()
{
   add(QString("class ").append(_block->Base::name()).append(";"));
}






/*!
 */
void Forward::outputDefinition()
{}






/*!
 *
 * @param block  
 *
 * @param parent  
 */
Forward::Forward(Class* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






/*!
 *
 * @param line  
 */
bool Forward::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
