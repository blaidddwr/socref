#include "cppqt_parse_function.h"
#include <QStack>
#include <QRegularExpression>
#include <socutil/sut_exceptions.h>
#include "cppqt_parse_base.h"
#include "cppqt_function.h"
#include "cppqt_blockfactory.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_template.h"
#include "cppqt_operator.h"
#include "cppqt_constructor.h"
#include "cppqt_destructor.h"
#include "cppqt_settings.h"



using namespace Sut;
using namespace CppQt::Parse;
//






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Function::outputComments()
{
   // Check to see if this is a defined function. 
   if ( _block )
   {
      // Add a comment section to output with the function block's description, then 
      // template arguments, then regular arguments, and then return argument. 
      add("/*!");
      add(makeComment(_block->description()));
      add(makeTemplateComments(_block));
      outputArgumentComments();
      outputReturnDescriptionComment();
      add(" */");
   }

   // Else this is not a defined function so add a special comment header 
   // highlighting that fact to output. 
   else add("/*! !!! UNKNOWN FUNCTION !!! */");
}






/*!
 * Implements _CppQt::Parse::Base_ interface. This implementation will also add 
 * comments to functions that only appear in the header file, such as abstract or 
 * signals. This will output nothing if this is not a defined function. 
 */
void Function::outputDeclaration()
{
   // Make sure this is a defined function. 
   if ( _block )
   {
      // If this object's function is abstract, a signal, or is default then add its 
      // comments to output. 
      if ( _block->isAbstract()
           || _block->type() == BlockFactory::SignalType
           || _block->isDefault()
           || _block->isDeleted() )
      {
         outputComments();
      }

      // Create a new string line with this object's function's template arguments in 
      // declarative form, if any. 
      QString line {makeTemplateArguments(_block,true)};
      if ( !line.isEmpty() ) line += QChar(' ');

      // Add any set flags of this object's function that come before the name. 
      if ( _block->isExplicit() ) line += QStringLiteral("explicit ");
      if ( _block->isVirtual() ) line += QStringLiteral("virtual ");
      if ( _block->isConstExpr() ) line += QStringLiteral("constexpr ");
      if ( _block->isStatic() ) line += QStringLiteral("static ");

      // Add the return type, then name, and then arguments of this object's function to 
      // the line. 
      line += makeReturnValue() + makeName() + makeArguments(true);

      // Add any proceeding flags this object's function has set to the line. 
      if ( _block->isConst() ) line += QStringLiteral(" const");
      if ( _block->isNoExcept() ) line += QStringLiteral(" noexcept");
      if ( _block->isOverride() ) line += QStringLiteral(" override");
      if ( _block->isFinal() ) line += QStringLiteral(" final");
      if ( _block->isAbstract() ) line += QStringLiteral(" = 0");
      if ( _block->isDefault() ) line += QStringLiteral(" = default");
      if ( _block->isDeleted() ) line += QStringLiteral(" = delete");

      // Add the closing semicolon to the line. 
      line += QStringLiteral(";");

      // Add the declaration line to output. 
      add(line);
   }
}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Function::outputDefinition()
{
   // Check to see if this is a defined function. 
   if ( _block )
   {
      // Create a new line with the template declaration for this object's function, if 
      // any. 
      QString line {makeTemplateDeclaration(_block)};
      if ( !line.isEmpty() ) line += QStringLiteral(" ");

      // Add the return type, then scoped name, and then arguments of this object's 
      // function to the line. 
      line += makeReturnValue() + makePreScope(_block) + makeName() + makeArguments(false);

      // Add any set flags of this object's function that belong in the definition to 
      // the line. 
      if ( _block->isConst() ) line += QStringLiteral(" const");
      if ( _block->isNoExcept() ) line += QStringLiteral(" noexcept");

      // If this object has any initializer lines then add the special colon character 
      // to the line. 
      if ( !_initializers.isEmpty() ) line += QStringLiteral(":");

      // Add the definition line to output. 
      add(line);
   }

   // Else this is not a defined function so add the static string definition to 
   // output. 
   else add(_definition);

   // Add all of this object's initializer lines to output. 
   for (auto line : _initializers) add(line);

   // If this object has no code lines then add an opening and closing bracket to 
   // output. 
   if ( _code.isEmpty() ) add("{}");

   // Else this object does have code lines so add them to output with an opening and 
   // closing bracket. 
   else
   {
      add("{");
      for (auto line : _code) add(line);
      add("}");
   }
}






/*!
 * Constructs a new defined function parser with the given function block and 
 * parent parser. 
 *
 * @param block The function block that defines this base function parser. 
 *
 * @param parent The parent parser for this new function parser. 
 */
Function::Function(CppQt::Function* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _maxColumns(Settings::instance().maxColumns())
{}






/*!
 * Constructs a new undefined function parser with the given static definition 
 * header and parent parser. 
 *
 * @param definition The static definition header for this new undefined function 
 *                   parser. This should include the entire line so the number of 
 *                   cut off spaces can be calculated. 
 *
 * @param parent The parent parser for this new function parser. 
 */
Function::Function(const QString& definition, AbstractParser* parent):
   Base(parent),
   _maxColumns(Settings::instance().maxColumns())
{
   // Determine the number of cut off spaces from the given definition header line 
   // and then set this object's static definition string. 
   _cutOff = definition.indexOf(QRegularExpression("\\S"));
   _definition = definition.mid(_cutOff);
}






/*!
 * Determines if the given line matches the definition header for this function 
 * parser, returning true if it is a match. If the function parser already has code 
 * or initializer lines, or is not a defined parser then false is always returned. 
 *
 * @param line The line presumed to be another function definition header that is 
 *             matched with this function parser. 
 *
 * @return True if the given line matches the definition header for this parser or 
 *         false if it does not, it has code or initializer lines, or is undefined. 
 */
bool Function::isMatch(const QString& line)
{
   // Make sure this object does not have code and is defined. 
   if ( hasCode() || !_definition.isEmpty() ) return false;

   // Create a new regular expression line with this object's function's name. 
   QString regular {QStringLiteral("\\A.*") + makeName(true) + QStringLiteral("\\(\\s*")};

   // Add any arguments of this object's function to the regular expression line. 
   for (auto argument : _block->arguments())
   {
      regular += argument->variableType().replace("*","\\*") + QStringLiteral(".*");
   }
   regular += QStringLiteral("\\)");

   // Add any set flags of this object's function that belong in the definition to 
   // the regular expression line. 
   if ( _block->isConst() ) regular += QStringLiteral("\\s+const");
   if ( _block->isNoExcept() ) regular += QStringLiteral("\\s+noexcept");

   // Add the closing syntax to the regular expression line. 
   regular += QStringLiteral("\\s*:?\\s*\\z");

   // Evaluate the line with the constructed regular expression line to determine a 
   // match, returning its result. 
   return QRegularExpression(regular).match(line).hasMatch();
}






/*!
 * Tests if this function parser has any code or initializer lines. 
 *
 * @return True if this function parser has any code or initializer lines or false 
 *         otherwise. 
 */
bool Function::hasCode() const
{
   return !_initializers.isEmpty() || !_code.isEmpty();
}






/*!
 * Sets the number of cut off spaces for this function parser to the given value. 
 * The value must be zero or positive. 
 *
 * @param value The new cut off value for this function parser. 
 */
void Function::setCutOff(int value)
{
   // Make sure the new value is valid. 
   if ( value < 0 )
   {
      Exception::InvalidArgument e;
      SUT_MARK_EXCEPTION(e);
      e.setDetails(tr("Cannot set cut off to invalid value %1.").arg(value));
      throw e;
   }

   // Set the new value. 
   _cutOff = value;
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

   // If this object is outside of any bracket level then add the given line to the 
   // object's initializer line list. 
   if ( _level == 0) _initializers << line.mid(_cutOff);

   // Else the given line is inside the main opening and closing brackets. 
   else
   {
      // Check to see if the given line is an inline comment. 
      if ( _block && QRegularExpression("\\A\\s*// .*\\z").match(line).hasMatch() )
      {
         // If the given inline comment line is the first for its grouping then set this 
         // object's edge past to true and process the inline comment. If this is 
         // proceeding inline comment lines after the first then just ignore it. 
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
         _code << line.mid(_cutOff);
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
   int spacing {line.indexOf(QRegularExpression("\\S")) - _cutOff};
   if ( spacing < 0 ) spacing = 0;

   // If the next operation iterator of this object is not past the end of this 
   // object's function's steps of operation then insert an inline comment with the 
   // next operation. 
   if ( _nextOperation < _block->operations().size() )
   {
      insertInlineComment(_nextOperation++,spacing);
   }

   // Else there are no more steps of operation to add as inline comments so add a 
   // blank inline comment to this object's code line list. 
   else _code << QString(spacing,' ') + QStringLiteral("// .");
}






/*!
 * Inserts a new inline comment grouping of lines to this object's code line list 
 * with the given steps of operation index and spacing. 
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
   QStringList words {_block->operations().at(index).split(QRegularExpression("\\s+"))};

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
      while ( !words.isEmpty() && (total + words.first().size() + 1) <= _maxColumns )
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
 * Adds argument comment lines of this object's function to output. This does not 
 * add opening or closing comment lines and assumes the output is already in the 
 * process of added comments. If this object's function has not arguments then 
 * nothing is added to output. 
 */
void Function::outputArgumentComments()
{
   // Iterate through all variable block children of this object's function block. 
   const QList<Variable*> arguments {_block->arguments()};
   for (auto argument : arguments)
   {
      // Add an empty comment line to output. 
      add(" *");

      // Create a base string that starts with the variable name and a space, getting 
      // the justified length from the base string length. 
      QString base {QStringLiteral("@param ") + argument->Base::name() + QStringLiteral(" ")};
      int justified {base.size()};

      // Add the variable block's description to the base string. 
      base += argument->description();

      // Make comments from the base string and the calculated justification and add its 
      // lines to output. 
      add(makeComment(base,justified));
   }
}






/*!
 * Adds return comment lines of this object's function to output. This does not add 
 * opening or closing comment lines and assumes the output is already in the 
 * process of added comments. If this object's function has no return description 
 * then nothing is added to output. 
 */
void Function::outputReturnDescriptionComment()
{
   // Get the return description of this object's function and make sure it is not 
   // empty. 
   const QString returnDescription {_block->returnDescription()};
   if ( returnDescription.isEmpty() ) return;

   // Add an empty comment line to output. 
   add(" *");

   // Create a base string with the return keyword in it, getting the justified 
   // length from the base string length. 
   QString base {"@return "};
   int justified {base.size()};

   // Add the return description to the base string. 
   base += returnDescription;

   // Make comments from the base string and calculated justification and add its 
   // lines to output. 
   add(makeComment(base,justified));
}






/*!
 * Makes and returns a string that is the return type for this object's function. 
 * The returned string also includes a space after the return type. 
 *
 * @return Return type for this object's function along with an extra space. 
 */
QString Function::makeReturnValue()
{
   // Create a new return string. 
   QString ret;

   // Check to see if this object's function is an operator type. 
   if ( _block->type() == BlockFactory::OperatorType )
   {
      // If this object's operator's name contains a space for the first character then 
      // it is an operator type that has no return declaration so return an empty 
      // string. 
      const QString name {_block->Base::name()};
      if ( !name.isEmpty() && name.at(0) == QChar(' ') ) return ret;
   }

   // If this object's function is a slot or signal then set the return to void. 
   if ( _block->type() == BlockFactory::SlotType || _block->type() == BlockFactory::SignalType )
   {
      ret = QStringLiteral("void ");
   }

   // Else this is a function with a return type so set the return string to that 
   // type with an additional space. 
   else if ( _block->type() != BlockFactory::ConstructorType
             && _block->type() != BlockFactory::DestructorType )
   {
      ret = _block->returnType() + QStringLiteral(" ");
   }

   // Return the return string. 
   return ret;
}






/*!
 * Makes and returns a string that is the function name for this object's function. 
 * This is just the name of the function, not including the return type or 
 * arguments. This can return a string intended for regular expression matching if 
 * the given flag is set to do so. 
 *
 * @param isRegExp True to return the name string with regular expression syntax or 
 *                 false to return a regular string. 
 *
 * @return The function name for this object's function, padded with regular 
 *         expression syntax is set to do so. 
 */
QString Function::makeName(bool isRegExp)
{
   // Create a new return string. 
   QString ret;

   // Check to see if this object's function is an operator. 
   if ( const Operator* valid = qobject_cast<const Operator*>(_block) )
   {
      // Set the return string to the operator keyword. 
      ret = QStringLiteral("operator");

      // If the regular expression flag is set then pad the operation characters of this 
      // object's operator. 
      if ( isRegExp ) ret += QRegularExpression::escape(valid->operation());

      // Else this is not a regular expression name so append the operation of this 
      // object's operator. 
      else ret += valid->operation();
   }

   // Else if this object's function is a destructor then set the return string 
   // appropriately. 
   else if ( const Destructor* valid = qobject_cast<const Destructor*>(_block) )
   {
      ret = QStringLiteral("~") + valid->className();
   }

   // Else if this object's function is a constructor then set the return string 
   // appropriately. 
   else if ( const Constructor* valid = qobject_cast<const Constructor*>(_block) )
   {
      ret = valid->className();
   }

   // Else this object's function is normal so set the return string to the base 
   // name. 
   else ret = _block->Base::name();

   // Return the function name string. 
   return ret;
}






/*!
 * Makes and returns the argument list for this object's function. The returned 
 * arguments string includes the opening and closing parenthesis. If the given flag 
 * is set to include any initializers of arguments then they are included in the 
 * return string. 
 *
 * @param withInitializers True to include the initializers of any arguments that 
 *                         have then in the returned arguments string or false to 
 *                         ignore them. 
 *
 * @return Argument list for this object's function, including the opening and 
 *         closing parenthesis and optional initializers if set to do so. 
 */
QString Function::makeArguments(bool withInitializers)
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
      ret += argument->variableType() + QStringLiteral(" ") + argument->Base::name();

      // If the with initializers flag is set and this argument has one then add it. 
      if ( withInitializers && argument->hasInitializer() )
      {
         ret += QStringLiteral(" = ") + argument->initializer();
      }
   }

   // Add the closing parenthesis. 
   ret += QStringLiteral(")");

   // Return the arguments string. 
   return ret;
}
