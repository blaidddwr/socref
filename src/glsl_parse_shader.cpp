#include "glsl_parse_shader.h"
#include <QRegularExpression>
#include "glsl_parse_variable.h"
#include "glsl_parse_struct.h"
#include "glsl_parse_function.h"
#include "glsl_shader.h"
#include "glsl_variable.h"
#include "glsl_struct.h"
#include "glsl_function.h"
#include "glsl_factory.h"
#include "glsl_settings.h"



using namespace GLSL::Parse;
//






/*!
 * Constructs a new shader root parser with the given shader block. 
 *
 * @param block The shader block that defines this new root parser. 
 */
Shader::Shader(const GLSL::Shader* block):
   _block(block)
{}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Shader::initialize()
{
   evaluate();
}






/*!
 * Implements _AbstractParser_ interface. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Shader::readLine(const QString& line)
{
   // If this is a preprocessor line then add it to this parser object's list of 
   // preprocessor lines. 
   if ( QRegularExpression(QStringLiteral("\\A#")).match(line).hasMatch() ) _preProcess << line;

   // Else check to see if the given line is a header of a function. 
   else if ( QRegularExpression(QStringLiteral("\\A\\s*[^#/].*\\([a-zA-Z0-9_, ]*\\)\\s*\\z")).match(line).hasMatch() )
   {
      // If this function is defined in this shader parser then step into it for read 
      // line processing. 
      if ( Function* child = findDefined(line) ) stepIntoChild(child);

      // Else check if this is an undefined function found in the shader source file. 
      else
      {
         // Add a new undefined function parser and then step into it for read line 
         // processing. 
         _children << new Function(line,this);
         stepIntoChild(_children.back());
      }
   }

   // Always return true because this root shader parser reads the entire source 
   // file. 
   return true;
}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Shader::makeOutput()
{
   // Add any preprocessor lines to output. 
   add(_preProcess);

   // Iterate through all parser children of this root parser. 
   for (auto child: qAsConst(_children))
   {
      // Add the blank lines and then the definition of the child parser to output. 
      child->outputLines();
      child->outputDefinition();
   }

   // Add the blank lines, comment header, definition header, and then definition of 
   // the special main function for the shader program file. 
   add(Settings::instance().functionLines());
   add(QStringLiteral("///"));
   addComment(_block->description());
   add(QStringLiteral("///"));
   add(QStringLiteral("void main()"));
   _main->outputDefinition();
}






/*!
 * Evaluates all children of this object's shader block, adding new child parser 
 * objects for each variable, struct, and function block it finds. 
 */
void Shader::evaluate()
{
   // Iterate through all children of this shader parser object's shader block. 
   for (auto child: _block->list())
   {
      // If the child is a variable block then make a new child parser and add it to 
      // this object's list of parser children. 
      if ( GLSL::Variable* valid = child->cast<GLSL::Variable>(GLSL::Factory::VariableType) )
      {
         _children << new Variable(valid,this);
      }

      // If the child is a struct block then make a new child parser and add it to this 
      // object's list of parser children. 
      else if ( GLSL::Struct* valid = child->cast<GLSL::Struct>(GLSL::Factory::StructType) )
      {
         _children << new Struct(valid,this);
      }

      // If the child is a function block then make a new child parser and add it to 
      // this object's list of parser children and its list of defined functions. 
      else if ( GLSL::Function* valid = child->cast<GLSL::Function>(GLSL::Factory::FunctionType) )
      {
         Function* parser {new Function(valid,this)};
         _children << parser;
         _defined << parser;
      }
   }

   // Lastly create a function parser for the special main function of this parser 
   // object's shader block. 
   _main = new Function(_block->operations(),this);
}






/*!
 * Attempts to find a defined function definition of this parser with the given 
 * definition string. If a match is found its function parser pointer is returned. 
 *
 * @param definition The function definition header whose match is looked for in 
 *                   this object's list of defined function parser objects. 
 *
 * @return Pointer of matched function parser if match is found or null if no match 
 *         is found. 
 */
Function* Shader::findDefined(const QString& definition)
{
   // If the given definition matches the special main function then return its 
   // pointer. 
   if ( _main->isMatch(definition) ) return _main;

   // Iterate through all defined function parser objects of this parser. 
   for (auto function : qAsConst(_defined))
   {
      // If the function parser matches the given definition header then return its 
      // pointer. 
      if ( function->isMatch(definition) ) return function;
   }

   // No match was found so return null. 
   return nullptr;
}
