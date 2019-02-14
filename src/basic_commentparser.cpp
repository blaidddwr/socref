#include "basic_commentparser.h"
#include <QRegularExpression>
#include "basic_lineparser.h"



using namespace Basic;
//






/*!
 * Implements _Abstract::Parser_ interface. 
 *
 * @return See interface docs. 
 */
QStringList CommentParser::output() const
{
   // Create the return list of comment lines, appending the begin line if it is not 
   // empty. 
   QStringList ret;
   if ( !_begin.isEmpty() ) ret << _begin;

   // Add empty comment lines equal to this parser element's header lines. 
   for (int i = 0; i < _headerLines ;++i) ret << _middle;

   // Iterate through all comment blocks of this parser element. 
   for (auto comment: _comments)
   {
      // Add the comment block and then its number of blank footer comment lines. 
      ret << createComment(comment.header,comment.text);
      for (int i = 0; i < comment.footerLines ;++i) ret << _middle;
   }

   // Add the end line if it is not empty and then return the generated comment block 
   // lines prepended with indent spaces. 
   if ( !_end.isEmpty() ) ret << _end;
   return LineParser::prependIndent(_indent,ret);
}






/*!
 * Constructs a new comment parser with the given begin line, middle token, end 
 * line, and maximum number of columns per line. 
 *
 * @param begin The begin line that is the first line provided by this comment 
 *              parser element or empty if no beginning line is required. 
 *
 * @param middle The middle token prepended to every comment line provided by this 
 *               comment parser element except for the special begin and end lines. 
 *
 * @param end The end line that is the last line provided by this comment parser 
 *            element or empty if no ending line is required. 
 *
 * @param maxColumns The maximum number of columns per comment line provided by 
 *                   this parser element. 
 */
CommentParser::CommentParser(const QString& begin, const QString& middle, const QString& end, int maxColumns):
   _begin(begin),
   _middle(middle),
   _end(end),
   _maxColumns(maxColumns)
{}






/*!
 * Sets the indent, in spaces, that is prepended to each line of the comment block 
 * this parser provides for its parent scanner. 
 *
 * @param value The new indent value in spaces. 
 */
void CommentParser::setIndent(int value)
{
   // Make sure the given indent is not negative. 
   Q_ASSERT(value >= 0);

   // Set the this object's indent to the new value. 
   _indent = value;
}






/*!
 * Adds the given comment used to generate and provide comment lines to this parser 
 * element's parent scanner. The given comment is wrapped into multiple lines to 
 * not exceed the maximum columns allowed per line. 
 *
 * @param text The commented added to this parser element's list of comments as a 
 *             new block. 
 */
void CommentParser::add(const QString& text)
{
   // Add a new comment block to the end of this parser element's list, setting its 
   // text to the given text. 
   _comments.push_back(Node());
   _comments.back().text = text;
}






/*!
 * Adds the given comment with a header used to generate and provide comment lines 
 * to this parser element's parent scanner. The header is added at the beginning of 
 * the first line, and each successive line is adjusted to start where the header 
 * ended. The given comment is wrapped into multiple lines to not exceed the 
 * maximum columns allowed per line. 
 *
 * @param header The special header text added to the beginning of the new comment. 
 *
 * @param text The main body text comment added to this parser element's list of 
 *             comments as a new block. 
 */
void CommentParser::add(const QString& header, const QString& text)
{
   // Add a new comment block to the end of this parser element's list, setting its 
   // header and text to the given header and text. 
   _comments.push_back(Node());
   _comments.back().header = header;
   _comments.back().text = text;
}






/*!
 * Adds a given number of blank comment lines that this parser will provide as 
 * output for its parent scanner, adding it to the number of blank lines for the 
 * last commend added or header lines if no comments have yet to be added. 
 *
 * @param count The number of blank lines added to the lines of this line parser. 
 */
void CommentParser::add(int count)
{
   // If there are no comment blocks yet then add the given count to this parser 
   // element's header lines, else add the given count to the footer lines of its 
   // last comment block. 
   if ( _comments.isEmpty() ) _headerLines += count;
   else _comments.back().footerLines += count;
}






/*!
 * Creates and returns a block of comment lines generated from the given header and 
 * text. The header can be empty, in that case it is ignored and the text is not 
 * justified. If a header is given the text will be justified to the right of the 
 * header after the first line. If the text is empty this returns an empty list. 
 *
 * @param header The optional header that the comment text if justified to the 
 *               right after the first comment line generated. 
 *
 * @param text The main body text used to generate the returned comment block 
 *             lines, wrapped to not exceed the maximum number of columns per line. 
 *
 * @return Block of comment lines generated from the given header and text. 
 */
QStringList CommentParser::createComment(QString header, const QString& text) const
{
   // Make sure the given text is not empty. 
   if ( text.isEmpty() ) return QStringList();

   // If the given header is not empty then append a single space to the end as a 
   // border between it and the comment text. 
   if ( !header.isEmpty() && header.back() != ' ' ) header += QStringLiteral(" ");

   // Initialize the justified length, returned comment line list, and first line 
   // indicator. 
   const int justified {header.size()};
   QStringList ret;
   bool first {true};

   // Split the given text into paragraphs using double newline characters as the 
   // separator and then iterate through all paragraphs. 
   QStringList paragraphs {text.split("\n\n",QString::SkipEmptyParts)};
   for (int i = 0; i < paragraphs.size() ;++i)
   {
      // Split the paragraph into its separate words. 
      QStringList words {paragraphs.at(i).split(QRegularExpression("\\s+"))};

      // Continue until the word list is empty. 
      while ( !words.isEmpty() )
      {
         // Initialize the total columns taken and new comment line to be added, middle 
         // token, and then another space. 
         int total {_middle.size() + 1};
         QString line {_middle + QStringLiteral(" ")};

         // Check to see if this is the first comment line. 
         if ( first )
         {
            // Add the header to the total columns taken and the comment line, updating the 
            // first indicator to false. 
            total += header.size();
            line += header;
            first = false;
         }

         // Else this is not the first comment line. 
         else
         {
            // Justify the words to be added to the right of the header on the first line. 
            total += justified;
            line += QString(justified,QChar(' '));
         }

         // Add the first word to to the total columns taken and the comment line. 
         total += words.first().size();
         line += words.takeFirst();

         // Continue until there are no more words to add or the maximum column size for 
         // the comment line has been reached. 
         while ( !words.isEmpty() && (total + words.first().size() + 1) <= _maxColumns )
         {
            // Add the next word to the comment line, updating the total columns taken. 
            total += words.first().size() + 1;
            line += QStringLiteral(" ") + words.takeFirst();
         }

         // Add the generated comment line to the line list. 
         ret << line;
      }

      // If this is not the last sentence then add an empty comment line to the line 
      // list. 
      if ( i != (paragraphs.size() - 1) ) ret << _middle;
   }

   // Return the generated comment block lines. 
   return ret;
}
