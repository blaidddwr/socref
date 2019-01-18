#include "glsl_parse_base.h"
#include "glsl_settings.h"



using namespace GLSL::Parse;
//






/*!
 * This interface outputs the number of blank lines that precedes this parser 
 * object's definition. The default implementation does nothing. 
 */
void Base::outputLines()
{}






/*!
 * This interface outputs the comments and definition of this parser. In the case 
 * of the special main function of a shader program the comment header is not added 
 * to output in this interface. The default implementation does nothing. 
 */
void Base::outputDefinition()
{}






/*!
 * Implements _AbstractParser_ interface. This implementation does nothing and is 
 * defined because some GLSL parser classes do not need to implement this. 
 */
void Base::makeOutput()
{}






/*!
 * Wrapper for the _AbstractParser_ method. 
 *
 * @param text The text that is used to make and added and formatted output lines. 
 *
 * @param justified The justification used for adding extra spaces before every 
 *                  output line after the first. 
 */
void Base::addComment(const QString& text, int justified)
{
   AbstractParser::addComment(QStringLiteral("/// "),text,justified,Settings::instance().maxColumns());
}
