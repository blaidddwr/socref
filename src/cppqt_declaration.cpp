#include "cppqt_declaration.h"



namespace CppQt
{






/*!
 * Implements _Abstract::Block_ interface.
 *
 * @return See interface docs.
 */
QString Declaration::name() const
{
   return line().section(' ',0,0) + QStringLiteral(" ...");
}






/*!
 * Returns the declaration line string of this declaration block.
 *
 * @return Declaration line string of this declaration block.
 */
QString Declaration::line() const
{
   return getString("line");
}

}
