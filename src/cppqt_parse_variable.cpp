#include "cppqt_parse_variable.h"
#include "cppqt_parse_base.h"
#include "cppqt_variable.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Variable::outputComments()
{
   // Add a beginning comment line, then the description of this object's variable, 
   // and then an end comment line to output. 
   add("/*!");
   add(makeComment(_block->description()));
   add(" */");
}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Variable::outputDeclaration()
{
   // Add the comment lines to output. 
   outputComments();

   // Create a new line string. 
   QString line;

   // Add any set or required flags of this object's variable. 
   if ( _block->isConstExpr() ) line += QStringLiteral("constexpr ");
   if ( _block->isStatic() ) line += QStringLiteral("static ");
   if ( _block->isMutable() ) line += QStringLiteral("mutable ");
   if ( line.isEmpty() && !_block->isMember() ) line += QStringLiteral("extern ");

   // Add this object's variable type and and then its name separated by a space. 
   line += _block->variableType() + QStringLiteral(" ") + _block->Base::name();

   // Finish the declaration line and add it to output. 
   finishOutput(&line
                ,_block->hasInitializer() && ( _block->isConstExpr() || ( _block->isMember() && !_block->isStatic() ) ));
}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Variable::outputDefinition()
{
   // Make sure this object's variable is static or not a class member. 
   if ( _block->isStatic() || !_block->isMember() )
   {
      // Create a new line with the template declaration for this object's variable. 
      QString line {makeTemplateDeclaration(_block)};
      if ( !line.isEmpty() ) line += QStringLiteral(" ");

      // Add this object's variable type and then its scoped name separated by a space. 
      line += _block->variableType()
            + QStringLiteral(" ")
            + makePreScope(_block)
            + _block->Base::name();

      // Finish the definition line and add it to output. 
      finishOutput(&line,_block->hasInitializer() && !_block->isConstExpr());
   }
}






/*!
 * Constructs a new variable parser with the given variable block and parent 
 * parser. 
 *
 * @param block The variable block that defines this new variable parser. 
 *
 * @param parent The parent parser for this new variable parser. 
 */
Variable::Variable(CppQt::Variable* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _maxColumns(Settings::instance().maxColumns()),
   _indentSpaces(Settings::instance().indentSpaces())
{}






/*!
 * Implements _AbstractParser_ interface. This interface does nothing because 
 * variables save nothing unique in their parsed files. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Variable::readLine(const QString& line)
{
   // Do nothing and return false. 
   Q_UNUSED(line)
   return false;
}






/*!
 * Finishes the given declaration or definition line for this object's variable, 
 * adding the initializer to the line if the given flag is set and this object's 
 * variable has an initializer value, and then adding it to output. 
 *
 * @param line Pointer to the declaration or definition line that is finished and 
 *             added to output. 
 *
 * @param withInitializer True to add this object's variable initializer to the 
 *                        given line if it has one or false to ignore any 
 *                        initializer value. 
 */
void Variable::finishOutput(QString* line, bool withInitializer)
{
   // Check to see if the with initializer flag is set to true and this object's 
   // variable has an initializer value. 
   if ( withInitializer && _block->hasInitializer() )
   {
      // Get the initializer value of this object's variable and check to see if the 
      // total column size of the line would be greater than the max columns setting. 
      const QString initialize {_block->initializer()};
      if ( (initialize.size() + line->size() + indent() + 1) > _maxColumns )
      {
         // Add the given line and then an open bracket to output, adding to the indent 
         // afterwords. 
         add(*line);
         add("{");
         setIndent(indent() + _indentSpaces);

         // Split the initializer value by the comma character and iterate through each 
         // part. 
         const QStringList parts {initialize.split(',')};
         bool first {true};
         for (auto part : parts)
         {
            // Add the initializer part to output with a comma added to the beginning if this 
            // is not the first part. 
            QString line;
            if ( first ) first = false;
            else line += QStringLiteral(",");
            line += part;
            add(line);
         }

         // Decrease the indent size and add a closing bracket with a semicolon to output. 
         setIndent(indent() - _indentSpaces);
         add("};");
      }

      // Else the total column size is not greater than the max columns setting so 
      // append the initializer value and then a semicolon, and then add the given line 
      // to output. 
      else
      {
         *line += QStringLiteral(" {") + initialize + QStringLiteral("};");
         add(*line);
      }
   }

   // Else the initializer flag is false or there is no initializer value so append a 
   // closing semicolon to the given line and then add it to output. 
   else
   {
      *line += QStringLiteral(";");
      add(*line);
   }
}
