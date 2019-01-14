#include "glsl_parse_base.h"



using namespace GLSL::Parse;
//






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
   AbstractParser::addComment(QStringLiteral("/// "),text,justified,80);//TODO;settings
}
