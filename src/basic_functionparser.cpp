#include "basic_functionparser.h"
#include <QRegularExpression>
#include "basic_lineparser.h"



namespace Basic
{






/*!
 * Implements _Abstract::Parser_ interface.
 *
 * @return See interface docs.
 */
QStringList FunctionParser::output() const
{
   // Copy this object's list of output lines, adding in and out token lines if it
   // only contains a single header line.
   QStringList ret {_lines};
   if ( _lines.size() == 1 ) ret << _inToken << _outToken;

   // Return the output lines prepended with this object's indent spacing.
   return LineParser::prependIndent(_indent,ret);
}






/*!
 * Implements _Abstract::Parser_ interface.
 *
 * @return See interface docs.
 */
bool FunctionParser::needsInput() const
{
   // Make sure input has not been read and a header regular expression is defined.
   return _first && !_headerExp.isEmpty();
}






/*!
 * Implements _Abstract::Parser_ interface.
 *
 * @return See interface docs.
 */
QString FunctionParser::headerExpression() const
{
   return _headerExp;
}






/*!
 * Implements _Abstract::Parser_ interface.
 *
 * @param value See interface docs.
 */
void FunctionParser::setIndent(int value)
{
   // Make sure the given indent is not negative.
   Q_ASSERT(value >= 0);

   // Set this object's indent to the new value.
   _indent = value;
}






/*!
 * Implements _Abstract::Parser_ interface.
 *
 * @param line See interface docs.
 *
 * @return See interface docs.
 */
bool FunctionParser::input(const QString& line)
{
   // Check to see if this is the first input line parsed by this object.
   if ( _first )
   {
      // Determine the cut off and set it to the instance wide cut off value.
      _cutOff = line.indexOf(QRegularExpression("\\S"));

      // Set the first line indicator to false and return true to get more input.
      _first = false;
      return true;
   }

   // Determine the cut off for this line based off the minimum of this lines spaces
   // or the instance wide cutoff.
   int cutOff {qMin(line.indexOf(QRegularExpression("\\S")),_cutOff)};

   // Check to see if the given line is an inline comment line.
   if ( QRegularExpression(QStringLiteral("\\A\\s*") + QRegularExpression::escape(_commentBegin)).match(line).hasMatch() )
   {
      // Check to see if this is the first line of an inline comment block.
      if ( !_edgePast )
      {
         // Mark the comment block edge past indicator to true and process this inline
         // comment block.
         _edgePast = true;
         processInlineComment(line.mid(cutOff));
      }
   }

   // Else the given line is not an inline comment.
   else
   {
      // Mark the edge past indicator to false for the next possible inline comment
      // block and add the given line to this object's output lines, cutting off the
      // extra initial white space.
      _edgePast = false;
      _lines += line.mid(cutOff);
   }

   // Determine the depth of execution block this given line ends with.
   int change {line.count(_inToken) - line.count(_outToken)};
   _depth += change;

   // If this input line is the end of this function in this parent scanner's source
   // file then return false to end getting input else return true to get more.
   if ( change < 0 && _depth <= 0 ) return false;
   else return true;
}






/*!
 * Implements _Abstract::Parser_ interface.
 */
void FunctionParser::reset()
{
   // Resets all instance variables used for processing input lines to their default
   // state.
   _first = true;
   _cutOff = 0;
   _depth = 0;
   _edgePast = false;
   _nextOperation = 0;

   // Clear any generated output lines parsed from previous input lines.
   _lines.clear();
   _lines << _header;
}






/*!
 * Constructs a new function parser with the given in token, out token, comment
 * begin string, maximum columns per inline comment, and steps of operation.
 *
 * @param inToken The character used by this parent scanner's source file to
 *                begin a block of execution.
 *
 * @param outToken The character used by this parent scanner's source file to
 *                 end a block of execution.
 *
 * @param commentBegin The characters used by this parent scanner's source file
 *                     to begin an inline comment line.
 *
 * @param maxColumns The maximum number of columns per line this parser will
 *                   generate for inline comment blocks.
 *
 * @param header The header or full name of this function that will be used as
 *               the first line of output for this function parser.
 *
 * @param operations A list of operation step descriptions this function parser
 *                   uses to generate inline comment block lines.
 */
FunctionParser::FunctionParser(QChar inToken, QChar outToken, const QString& commentBegin, int maxColumns, const QString& header, const QStringList& operations)
   :
   _inToken(inToken),
   _outToken(outToken),
   _commentBegin(commentBegin),
   _maxColumns(maxColumns),
   _header(header),
   _operations(operations),
   _lines(header)
{
}






/*!
 * Appends the given text to the header regular expression of this function
 * parser, escaping all characters to be taken literally by the regular
 * expression interpreter.
 *
 * @param text Text that is escaped and then appended to the header expression
 *             of this function parser.
 */
void FunctionParser::add(const QString& text)
{
   _headerExp += QRegularExpression::escape(text);
}






/*!
 * Appends the given expression to the header regular expression of this
 * function parser. The given expression is not escaped and appended without
 * modification to the header.
 *
 * @param expression Expression that is appended to the header expression of
 *                   this function parser.
 */
void FunctionParser::addExp(const QString& expression)
{
   _headerExp += expression;
}






/*!
 * Processes the given inline comment line, adding the next step of operations
 * of this function parser as an inline comment to this object's output lines or
 * a blank inline comment if there are no more steps of operation to use.
 *
 * @param line The inline comment given as an input line from the input
 *             interface.
 */
void FunctionParser::processInlineComment(const QString& line)
{
   // Calculate the number of spaces that should be used for this inline comment
   // grouping.
   int spacing {line.indexOf(QRegularExpression("\\S"))};

   // If the next operation iterator of this object is not past the end of this
   // object's steps of operation then insert an inline comment with the next
   // operation.
   if ( _nextOperation < _operations.size() )
   {
      insertInlineComment(_nextOperation++,spacing);
   }

   // Else there are no more steps of operation to add as inline comments so add a
   // blank inline comment to this object's output lines.
   else _lines << QString(spacing,' ') + QStringLiteral("// .");
}






/*!
 * Inserts new inline comment lines to this object's output lines with the given
 * steps of operation index and spacing.
 *
 * @param index The index of the operation in this object's steps of operation
 *              used to generate the inline comment.
 *
 * @param spacing The number of spaces added before every inline comment line
 *                generated.
 */
void FunctionParser::insertInlineComment(int index, int spacing)
{
   // Split operation from this object's steps of operation with the given index into
   // a list of words.
   QStringList words {_operations.at(index).split(QRegularExpression("\\s+"))};

   // Keep going until the list of words is empty.
   while ( !words.isEmpty() )
   {
      // Create a total columns used variable with the first word's size plus one space.
      int total {words.first().size() + 1};

      // Create a new line string, adding spaces then comment characters then the first
      // word from the list.
      QString line {QString(spacing,' ') + _commentBegin + words.takeFirst()};

      // Keep going until the word list is empty or the line has reached the maximum
      // number of columns.
      while ( !words.isEmpty() && (total + words.first().size() + 1) <= _maxColumns )
      {
         // Take and add the next word from the list, adding the number of columns the
         // addition used.
         total += words.first().size() + 1;
         line += QStringLiteral(" ") + words.takeFirst();
      }

      // Add the inline comment line to this parser element's output list.
      _lines << line;
   }
}

}
