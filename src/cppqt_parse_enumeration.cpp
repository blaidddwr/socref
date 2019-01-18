#include "cppqt_parse_enumeration.h"
#include "cppqt_parse_base.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Enumeration::outputComments()
{
   // Add the description comment for this object's enumeration to output with 
   // opening and closing comment lines. 
   add("/*!");
   add(makeComment(_block->description()));
   add(" */");
}






/*!
 * Implements _CppQt::Parse::Base_ interface. 
 */
void Enumeration::outputDeclaration()
{
   // Add the comments to output. 
   outputComments();

   // Create a new line starting with the enumeration declaration, adding a class 
   // qualifier if this object's enumeration is a class type. 
   QString line {"enum "};
   if ( _block->isClass() ) line.append("class ");

   // Append the base name of this object's enumeration. 
   line += _block->baseName();

   // Add the enumeration header line, then an opening bracket line, and then 
   // increment the indent. 
   add(line);
   add("{");
   setIndent(indent() + _indentSpaces);

   // Iterate through all enumeration value block children of this object's 
   // enumeration. 
   const QList<EnumValue*> list {_block->values()};
   bool first {true};
   for (auto value : list)
   {
      // Add the comments of the enumeration value block to output with opening and 
      // closing comment lines. 
      add("/*!");
      add(makeComment(value->description()));
      add(" */");

      // Create a new line, adding a comma if this is not the first enumeration value 
      // child block. 
      QString line;
      if ( first ) first = false;
      else line.append(",");

      // Append the base name of the enumeration value block. 
      line += value->baseName();

      // If the enumeration value block has a value set then append it to the line. 
      if ( value->hasValue() ) line += QStringLiteral(" = ") + value->value();

      // Add the enumeration value declaration line to output. 
      add(line);
   }

   // Decrease the indent by one and add a closing bracket line to output with a 
   // semicolon. 
   setIndent(indent() - _indentSpaces);
   add("};");
}






/*!
 * Implements _CppQt::Parse::Base_ interface. This does nothing because enumeration 
 * definitions do not exist. 
 */
void Enumeration::outputDefinition()
{}






/*!
 * Constructs a new enumeration parser with the given enumeration block and parent 
 * parser. 
 *
 * @param block The enumeration block that defines this new enumeration parser. 
 *
 * @param parent The parent parser for this new enumeration parser. 
 */
Enumeration::Enumeration(CppQt::Enumeration* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






/*!
 * Implements _AbstractParser_ interface. This interface does nothing because 
 * nothing unique is saved in their parsed files. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Enumeration::readLine(const QString& line)
{
   // Do nothing and return false. 
   Q_UNUSED(line)
   return false;
}
