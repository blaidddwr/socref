#include "glsl_parse_function.h"
#include <QRegularExpression>
#include "glsl_function.h"
#include "glsl_variable.h"



using namespace GLSL::Parse;
//






/*!
 * Implements _AbstractParser_ interface. 
 */
void Function::makeOutput()
{
   // Check if this is a defined function. 
   if ( _block )
   {
      // Make comment header output and then add the definition line to output. 
      makeCommentOutput();
      add(_block->returnType() + QStringLiteral(" ") + _block->baseName() + makeArguments());
   }

   // Else if this is an undefined function make a simple comment header stating as 
   // such. 
   else if ( !_definition.isEmpty() ) add(QStringLiteral("/// !!! UNKNOWN FUNCTION !!!"));

   // Else if this is not a defined function then add the static string definition to 
   // output. 
   else if ( !_definition.isEmpty() ) add(_definition);

   // If this object has no code lines then add an opening and closing bracket to 
   // output. 
   if ( _code.isEmpty() ) add("{}");

   // Else this object does have code lines so add them to output with an opening and 
   // closing bracket. 
   else
   {
      add("{");
      add(_code);
      add("}");
   }
}






/*!
 * Constructs a new function parser as undefined with the given static definition 
 * header and parent parser. 
 *
 * @param definition Static definition header for this new undefined function 
 *                   parser. 
 *
 * @param parent The parent parser object for this new function parser. 
 */
Function::Function(const QString& definition, AbstractParser* parent):
   Base(parent),
   _definition(definition)
{}






/*!
 * Constructs a new function parser as defined with the given function block and 
 * parent parser. 
 *
 * @param block The function block that defines this new function parser. 
 *
 * @param parent The parent parser object for this new function parser. 
 */
Function::Function(const GLSL::Function* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _operations(_block->operations())
{}






/*!
 * Constructs a new function parser as the special main function of a shader 
 * program file with the given steps of operation and parent parser. 
 *
 * @param operations The steps of operation for the special main function of this 
 *                   parser object's shader source file. 
 *
 * @param parent The parent parser object for this new function parser. 
 */
Function::Function(const QStringList& operations, AbstractParser* parent):
   Base(parent),
   _operations(operations)
{}






/*!
 * Determines if the given line matches the definition header for this function 
 * parser, returning true if it is a match. If the function parser already has code 
 * or is not a defined parser then false is always returned. 
 *
 * @param line The line presumed to be another function definition header that is 
 *             matched with this function parser. 
 *
 * @return True if the given line matches the definition header for this parser or 
 *         false if it does not, it has code, or is undefined. 
 */
bool Function::isMatch(const QString& line)
{
   // Make sure this object does not have code. 
   if ( !_code.isEmpty() ) return false;

   // Check to see if this parser object does not have a valid function block 
   // pointer. 
   if ( !_block )
   {
      // If this parser is undefined then return false else this is the special main 
      // function so match the given line against that definition. 
      if ( !_definition.isEmpty() ) return false;
      else return QRegularExpression(QStringLiteral("main\\(\\)\\s*\\z")).match(line).hasMatch();
   }

   // Create a new regular expression line with this object's function's name. 
   QString regular {QRegularExpression::escape(_block->baseName()) + QStringLiteral("\\(")};

   // Add any arguments of this object's function to the regular expression line. 
   bool first {true};
   for (auto argument : _block->arguments())
   {
      if ( !first ) regular += QStringLiteral(",\\s*");
      else first = false;
      regular += QRegularExpression::escape(argument->typeString()) + QStringLiteral("[^,]*");
   }
   regular += QStringLiteral("\\)");

   // Add the closing syntax to the regular expression line. 
   regular += QStringLiteral("\\s*\\z");

   // Evaluate the line with the constructed regular expression line to determine a 
   // match, returning its result. 
   return QRegularExpression(regular).match(line).hasMatch();
}






/*!
 * Implements _AbstractParser_ interface. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Function::readLine(const QString& line)
{
   // If the given line is a opening and closing bracket that means this function has 
   // no code lines so return false. 
   if ( _level == 0 && QRegularExpression("\\A\\s*\\{\\}\\s*\\z").match(line).hasMatch() )
   {
      return false;
   }

   // Check to see if the given line is an opening bracket. 
   if ( QRegularExpression("\\A\\s*\\{\\s*\\z").match(line).hasMatch() )
   {
      // Increment the bracket level by one, returning true if this is the very first 
      // opening bracket. 
      ++_level;
      if ( _level == 1 ) return true;
   }

   // Check to see if the given line is a closing bracket. 
   else if ( QRegularExpression("\\A\\s*\\};?\\s*\\z").match(line).hasMatch() )
   {
      // Decease the bracket level by one, returning false if this is the final closing 
      // bracket. 
      --_level;
      if ( _level == 0 ) return false;
   }

   // Else check to see if the given line is inside the main opening and closing 
   // brackets. 
   else if ( _level > 0 )
   {
      // Check to see if the given line is an inline comment. 
      if ( _block && QRegularExpression("\\A\\s*// .*\\z").match(line).hasMatch() )
      {
         // If the given inline comment line is the first for its grouping then set this 
         // object's edge past to true and process the inline comment, if this is a 
         // proceeding inline comment line after the first then just ignore it. 
         if ( !_edgePast )
         {
            _edgePast = true;
            processInlineComment(line);
         }
      }

      // Else the given line is regular code so set this object's edge past to false and 
      // add the given line to this object's code line list. 
      else
      {
         _edgePast = false;
         _code << line;
      }
   }

   // The given line is not the last line of code for this function so return true. 
   return true;
}






/*!
 * Processes the given inline comment line, adding the next step of operations of 
 * this object's function as an inline comment to this object's code line list or a 
 * blank inline comment if there are no more steps of operation to use. 
 *
 * @param line The inline comment given as an input line from the read line 
 *             interface. The line must not be modified so the number of spaces can 
 *             be calculated. 
 */
void Function::processInlineComment(const QString& line)
{
   // Calculate the number of spaces that should be used for this inline comment 
   // grouping. 
   int spacing {line.indexOf(QRegularExpression("\\S"))};
   if ( spacing < 0 ) spacing = 0;

   // If the next operation iterator of this object is not past the end of this 
   // object's function's steps of operation then insert an inline comment with the 
   // next operation. 
   if ( _nextOperation < _operations.size() )
   {
      insertInlineComment(_nextOperation++,spacing);
   }

   // Else there are no more steps of operation to add as inline comments so add a 
   // blank inline comment to this object's code line list. 
   else _code << QString(spacing,' ') + QStringLiteral("// .");
}






/*!
 * Inserts a new inline comment to this object's code line list with the given 
 * steps of operation index and spacing, separating the comment into as many number 
 * of lines as needed. 
 *
 * @param index The index of the operation in this object's function's steps of 
 *              operation to use to generate the inline comment. 
 *
 * @param spacing The number of spaces added before every inline comment line 
 *                generated. 
 */
void Function::insertInlineComment(int index, int spacing)
{
   // Split operation from this object's function's steps of operation with the given 
   // index into a list of words. 
   QStringList words {_operations.at(index).split(QRegularExpression("\\s+"))};

   // Keep going until the list of words is empty. 
   while ( !words.isEmpty() )
   {
      // Create a total columns used variable with the first word's size plus one space. 
      int total {words.first().size() + 1};

      // Create a new line string, adding spaces then comment characters then the first 
      // word from the list and then a space. 
      QString line
      {
         QString(spacing,' ') + QStringLiteral("// ") + words.takeFirst() + QStringLiteral(" ")
      };

      // Keep going until the word list is empty or the line has reached the maximum 
      // number of columns. 
      while ( !words.isEmpty() && (total + words.first().size() + 1) <= 80 )//TODO: max columns setting
      {
         // Take and add the next word from the list to the line and an additional space, 
         // adding the number of columns the addition used. 
         total += words.first().size() + 1;
         line += words.takeFirst() + QStringLiteral(" ");
      }

      // Add the inline comment line to this object's code line list. 
      _code << line;
   }
}






/*!
 * This adds the comment header of a defined function to this parser object's 
 * output lines. This parser must contain a valid pointer to a valid function block 
 * or its behavior is undefined. 
 */
void Function::makeCommentOutput()
{
   // Add an empty comment line and then the function's description. 
   add(QStringLiteral("///"));
   addComment(_block->description());

   // Iterate through all variable blocks of this parser object's function block. 
   const QList<Variable*> arguments {_block->arguments()};
   for (auto argument : arguments)
   {
      // Add an empty comment line to output. 
      add(QStringLiteral("///"));

      // Create a base string that starts with the variable name and a space, getting 
      // the justified length from the base string length. 
      QString comment {QStringLiteral("@param ") + argument->baseName() + QStringLiteral(" ")};
      int justified {comment.size()};

      // Add comments from the base string and the variable's description. 
      addComment(comment + argument->description(),justified);
   }

   // If this parser object's function block does not have a void return then add the 
   // description for the return variable. 
   if ( !_block->isVoidReturn() )
   {
      add(QStringLiteral("///"));
      addComment(QStringLiteral("@return ") + _block->returnDescription(),7);
   }

   // Finish by adding an empty comment line. 
   add(QStringLiteral("///"));
}






/*!
 * Makes and returns the argument list for this object's function. The returned 
 * arguments string includes the opening and closing parenthesis. This parser must 
 * contain a valid pointer to a valid function block or its behavior is undefined. 
 *
 * @return Argument list for this object's function, including the opening and 
 *         closing parenthesis. 
 */
QString Function::makeArguments()
{
   // Create a new return string with the opening parenthesis. 
   QString ret {QStringLiteral("(")};

   // Iterate through all arguments of this object's function block. 
   bool first {true};
   for (auto argument : _block->arguments())
   {
      // If this is not the first argument then add a separating comma and space. 
      if ( first ) first = false;
      else ret += QStringLiteral(", ");

      // Append the argument type, a space, and its name. 
      ret += argument->typeString() + QStringLiteral(" ") + argument->baseName();
   }

   // Add the closing parenthesis. 
   ret += QStringLiteral(")");

   // Return the arguments string. 
   return ret;
}
