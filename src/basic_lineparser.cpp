#include "basic_lineparser.h"



using namespace Basic;
//






/*!
 * Prepends the given number of indent spaces to each line of the given list of 
 * lines, returning the prepended list of lines. 
 *
 * @param indent The number of spaces prepended to each line in the given line 
 *               list. 
 *
 * @param lines The list of lines used to create and return a new list of lines 
 *              prepended with indent spaces for each line. 
 *
 * @return List of lines prepended with the given number of indent spaces for each 
 *         line. 
 */
QStringList LineParser::prependIndent(int indent, const QStringList& lines)
{
   // Create a returned list of lines. 
   QStringList ret;

   // Iterate through all lines of this object's line of lines. 
   for (auto line: lines)
   {
      // Make a copy of the line, prepending indent spaces to the copy if it is not an 
      // empty line. 
      QString temp {line};
      if ( !temp.isEmpty() ) temp.prepend(QString(indent,' '));

      // Add the copy to the returned list of lines. 
      ret << temp;
   }

   // Return the list of lines with prepended indent spaces. 
   return ret;
}






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @return See interface docs. 
 */
QStringList LineParser::output() const
{
   return prependIndent(_indent,_lines);
}






/*!
 * Constructs a new line parser initializes with the given number of blank lines. 
 *
 * @param count The number of blank lines added to this new parser. 
 */
LineParser::LineParser(int count)
{
   add(count);
}






/*!
 * Constructs a new line parser initialized with the given indent and single line. 
 *
 * @param indent The indent spacing of this new parser. 
 *
 * @param line The first line added to this new parser. 
 */
LineParser::LineParser(int indent, const QString& line)
{
   // Set this parser element's indent and its first line to the ones given. 
   setIndent(indent);
   add(line);
}






/*!
 * Sets the indent in spaces that is prepended to each line when this line parser 
 * provides its output lines to its parent scanner. 
 *
 * @param value The new indent value in spaces. 
 */
void LineParser::setIndent(int value)
{
   // Make sure the given indent is not negative. 
   Q_ASSERT(value >= 0);

   // Set this object's indent to the new value. 
   _indent = value;
}






/*!
 * Appends the given line to this parser element's output lines. 
 *
 * @param line The line that is appended to the output lines of this parser. 
 */
void LineParser::add(const QString& line)
{
   _lines << line;
}






/*!
 * Appends the given list of lines to this parser element's output lines. 
 *
 * @param lines Lines that are appended to the output lines of this parser. 
 */
void LineParser::add(const QStringList& lines)
{
   _lines << lines;
}






/*!
 * Appends a given number of blank lines to this parser element's output lines. 
 *
 * @param count The number of blank lines appended to the output lines of this 
 *              parser. 
 */
void LineParser::add(int count)
{
   // Make sure the given count is not negative. 
   Q_ASSERT(count >= 0);

   // Add the given number of blank lines to this object's list of lines. 
   while ( count-- > 0 ) _lines << QString();
}
