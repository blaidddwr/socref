#include "basic_inputparser.h"
#include <QRegularExpression>



using namespace Basic;
//






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @return See interface docs. 
 */
QStringList InputParser::output() const
{
   return _lines;
}






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @return See interface docs. 
 */
bool InputParser::needsInput() const
{
   return _lines.isEmpty();
}






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @return See interface docs. 
 */
QString InputParser::headerExpression() const
{
   return _beginExp;
}






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool InputParser::input(const QString& line)
{
   // Check to see if this is the first line of input therefore the matched beginning 
   // line. 
   if ( _first )
   {
      // Set this object's first indicator to false and return true so the beginning 
      // line is not added. 
      _first = false;
      return true;
   }

   // Make sure this is not a matching end line. 
   QRegularExpression regexp(_endExp);
   if ( regexp.match(line).hasMatch() ) return false;

   // Add the given line to this object's output lines and return true for more 
   // input. 
   _lines << line;
   return true;
}






/*!
 * Implements _Abstract::Parser_ interface. 
 */
void InputParser::reset()
{
   // Clear any output lines added from input and set this object's first line 
   // indicator to true. 
   _lines.clear();
   _first = true;
}






/*!
 * Constructs a new input parser with the given beginning line expression, ending 
 * line expression, and first line setting. 
 *
 * @param beginExp The regular expression used to match the beginning line that 
 *                 causes this parser to start reading input. 
 *
 * @param endExp The regular expression used to match the ending line that causes 
 *               this parser to end taking input. 
 *
 * @param first True to set this new input parser to read in the first line of 
 *              input or false to set it to ignore the first line. 
 */
InputParser::InputParser(const QString& beginExp, const QString& endExp, bool first):
   _beginExp(beginExp),
   _endExp(endExp),
   _first(!first)
{}
