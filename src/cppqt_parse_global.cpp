#include "cppqt_parse_global.h"
#include <QStack>
#include "cppqt_parse_base.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_factory.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Constructs a new global parser with the given namespace block as its root. 
 *
 * @param block The namespace block used as the root of this parser representing 
 *              its file. 
 */
Global::Global(const Namespace* block):
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






/*!
 * Implements _AbstractParser_ interface. 
 *
 * @param line  
 *
 * @return See interface docs. 
 */
bool Global::readLine(const QString& line)
{
   // This parser only overwrites its file so do nothing and return false. 
   Q_UNUSED(line)
   return false;
}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Global::makeOutput()
{
   // Add a single blank line to output. 
   add(1);

   // Add the namespace declarations and open brackets to output, excluding the 
   // namespace of this parser. 
   beginNamespaceNesting(false);

   // If the namespace of this parser is not the root namespace then add it's 
   // comments to output. 
   if ( _block->parent() )
   {
      add("/*!");
      add(Base::makeComment(_block->description()));
      add(" */");
   }

   // If the namespace of this parser is not the root block then add it. 
   if ( _block->parent() ) outputNamespace(_block);

   // Iterate through a list of all children class blocks of the namespace of this 
   // parser. 
   QList<Class*> list {_block->makeListOfType<Class>(Factory::ClassType)};
   for (auto item : list)
   {
      // If the class block has no templates then add its forward declaration to output. 
      if ( !item->hasTemplates() ) add(QString("class ").append(item->baseName()).append(";"));
   }

   // Add all closing brackets for all namespace declarations to output. 
   endNamespaceNesting();
}






/*!
 * Adds namespace declarations to output with the opening bracket but NO closing 
 * bracket. This also adds indents for each new namespace declaration. This can 
 * ignore the very last namespace, not adding its declaration, if the given flag 
 * says so. The last namespace is the namespace of this parser. 
 *
 * @param outputLast True to make the last namespace declaration, the namespace of 
 *                   this parser, added to output or false to ignore it. 
 */
void Global::beginNamespaceNesting(bool outputLast)
{
   // Create a pointer stack. 
   QStack<const Namespace*> scope;

   // Iterate through the parents of this parser object's namespace, starting with 
   // itself. 
   const AbstractBlock* block {_block};
   while ( block->parent() )
   {
      // If the current parent is a namespace then push it onto the stack. 
      if ( const Namespace* valid = block->cast<Namespace>(Factory::NamespaceType) )
      {
         scope.push(valid);
      }

      // Move to the next parent. 
      block = block->parent();
   }

   // Iterate through the stack, popping each namespace and adding its declaration to 
   // output. Only add the last namespace block's declaration if the given flag is 
   // set correctly. 
   while ( scope.size() > 1 || ( outputLast && !scope.isEmpty() ) ) outputNamespace(scope.pop());
}






/*!
 * Adds the closing brackets for all namespace declarations to output. This also 
 * removes the indenting for each opening bracket. 
 */
void Global::endNamespaceNesting()
{
   // While the depth is greater than zero. 
   while ( _depth > 0 )
   {
      // Remove a single indent spacing from this parser. 
      setIndent(indent() - _indentSpaces);

      // Output a closing bracket line and decrease the depth by one. 
      add("}");
      --_depth;
   }
}






/*!
 * Adds a single namespace declaration to output using the given namespace block. 
 * This also adds indentation after the opening bracket. This does NOT add the 
 * closing bracket. 
 *
 * @param block  
 */
void Global::outputNamespace(const Namespace* block)
{
   // Add the namespace declaration line and then an opening bracket line to output. 
   add(QString("namespace ").append(block->baseName()));
   add("{");

   // Add a single indent spacing to this parser and increment the depth. 
   setIndent(indent() + _indentSpaces);
   ++_depth;
}
