#include "abstract_parser.h"



using namespace Abstract;
//






/*!
 * This interface returns true if this parser element requires input from the 
 * source file of its parent scanner. This can be updated to false if this no 
 * longer needs input but cannot be changed from false to true. The default 
 * implementation returns false. 
 *
 * @return True if this parser element requires input from its parent scanner 
 *         source file or false otherwise. 
 */
bool Parser::needsInput() const
{
   return false;
}






/*!
 * This interface returns a regular expression that its parent scanner uses to 
 * match input lines of its source file. Once this object's parent scanner finds a 
 * match with the returned regular expression it is fed input lines until this 
 * object is finished. The default implementation returns an empty string. 
 *
 * @return Regular expression that its parent scanner uses to match input lines of 
 *         its source file. 
 */
QString Parser::headerExpression() const
{
   return QString();
}






/*!
 * This interface reads in one line of input given from its parent scanner, 
 * returning true if this parser element wants to read the next line after the one 
 * given. The default implementation returns false. 
 *
 * @param line One line of input given from this parser element's parent scanner. 
 *
 * @return True to signal this parser will continue reading input lines or false to 
 *         signal it is finished. 
 */
bool Parser::input(const QString& line)
{
   Q_UNUSED(line);
   return false;
}






/*!
 * This interface clears any saved information this parser element contains and 
 * resets it to its initial state. The default implementation does nothing. 
 */
void Parser::reset()
{}
