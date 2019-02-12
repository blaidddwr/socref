#include "basic_lineparser.h"



using namespace Basic;
//






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @return See interface docs. 
 */
QStringList LineParser::output() const
{
   // Create a returned list of lines. 
   QStringList ret;

   // Iterate through all lines of this object's line of lines. 
   for (auto line: _lines)
   {
      // Make a copy of the line, prepending indent spaces to the copy if it is not an 
      // empty line. 
      QString temp {line};
      if ( !temp.isEmpty() ) temp.prepend(QString(_indent,' '));

      // Add the copy to the returned list of lines. 
      ret << temp;
   }

   // Return the list of lines with prepended indent spaces. 
   return ret;
}






/*!
 * Sets the indent, in spaces, that is prepended to each line when this line parser 
 * generates its output for its parent scanner. 
 *
 * @param indent The new indent value in spaces. 
 */
void LineParser::setIndent(int indent)
{
   // Make sure the given indent is not negative. 
   Q_ASSERT(indent >= 0);

   // Set the this object's indent to the new value. 
   _indent = indent;
}






/*!
 * Adds the given line as a new line that this line parser will provide as output 
 * for its parent scanner, appending it to the end of any existing lines to be 
 * used. 
 *
 * @param line The line that is appended to the lines of this line parser. 
 */
void LineParser::add(const QString& line)
{
   _lines << line;
}






/*!
 * Adds the given list of lines as a new lines that this line parser will provide 
 * as output for its parent scanner, appending it to the end of any existing lines 
 * to be used. 
 *
 * @param lines Lines that are appended to the lines of this line parser. 
 */
void LineParser::add(const QStringList& lines)
{
   _lines << lines;
}






/*!
 * Adds a given number of blank lines that this line parser will provide as output 
 * for its parent scanner, appending it to the end of any existing lines to be 
 * used. 
 *
 * @param count The number of blank lines added to the lines of this line parser. 
 */
void LineParser::add(int count)
{
   // Make sure the given count is not negative. 
   Q_ASSERT(count >= 0);

   // Add the given number of blank lines to this object's list of lines. 
   while ( count-- > 0 ) _lines << QString();
}
