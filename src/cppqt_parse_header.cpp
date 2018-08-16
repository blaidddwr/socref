#include "cppqt_parse_header.h"
#include <QStack>
#include <QRegularExpression>
#include "cppqt_parse_function.h"
#include "cppqt_parse_base.h"
#include "cppqt_parse_variable.h"
#include "cppqt_parse_enumeration.h"
#include "cppqt_parse_access.h"
#include "cppqt_parse_forward.h"
#include "cppqt_function.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_blockfactory.h"
#include "cppqt_access.h"
#include "cppqt_class.h"
#include "cppqt_parent.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Constructs a new header parser with the given namespace block as its root and 
 * the name of the header file is parses. 
 *
 * @param block The namespace block this parser uses as its root. 
 *
 * @param name The name, excluding the extension, of the header file this object 
 *             parses. 
 */
Header::Header(const Namespace* block, const QString& name):
   Source(block),
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces()),
   _headerLines(Settings::instance().headerLines())
{
   // Create the two preprocessor lines that guard this header file from being 
   // included more than once. 
   QString define {name.toUpper().append("_H")};
   _guard1 = QString("#ifndef ").append(define);
   _guard2 = QString("#define ").append(define);
}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Header::makeOutput()
{
   // Add the preprocessor header guard lines to output. 
   outputGuard();

   // Add the preprocessor section to output. 
   outputPreProcessor();

   // Add the header section and then the end of top marker to output. 
   outputHeader();
   add("//");

   // Add blank header lines and then the opening bracket namespace nesting to 
   // output. 
   add(_headerLines);
   beginNamespaceNesting();

   // Add the declaration and then the definition sections to output. 
   outputDeclarations();
   outputDefinitions();

   // Add the closing bracket namespace and then the final preprocessor header guard 
   // line to output. 
   endNamespaceNesting();
   outputFooter();
}






/*!
 * Implements _CppQt::Parse::Source_ interface. 
 *
 * @param line See interface docs. 
 */
void Header::readTop(const QString& line)
{
   // If this is a custom preprocessor line that is not a header guard line then add 
   // it as a custom preprocessor line. 
   if ( QRegularExpression("\\A#.*\\z").match(line).hasMatch()
        && line != _guard1
        && line != _guard2 )
   {
      addPreProcess(line);
   }

   // Else if this is a custom forward class declaration then add it as a custom 
   // header line. 
   else if ( QRegularExpression("\\A\\s*class\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*\\z").match(line).hasMatch() )
   {
      addHeader(line.trimmed());
   }
}






/*!
 * Implements _CppQt::Parse::Source_ interface. 
 *
 * @param block See interface docs. 
 */
void Header::evaluateVariable(CppQt::Variable* block)
{
   // Create a new variable parser from the given variable block and append it to 
   // this object's declarations. 
   Variable* base {new Variable(block,this)};
   _declarations.append(base);

   // If the given variable block should have its definition in its header file then 
   // add its parser to this parser. 
   if ( block->isStatic() && !block->isConstExpr() && isTemplate() ) addVariable(base);
}






/*!
 * Implements _CppQt::Parse::Source_ interface. 
 *
 * @param block See interface docs. 
 */
void Header::evaluateFunction(CppQt::Function* block)
{
   // Create a new function parser from the given function block and append it to 
   // this object's declarations. 
   Function* base {new Function(block,this)};
   _declarations.append(base);

   // If the given block has the correct attributes to be defined in the header file 
   // then add its parser to the definitions section. 
   if ( !block->isDefault()
        && !block->isDeleted()
        && block->type() != BlockFactory::SignalType
        && ( isTemplate() || ( block->hasTemplates() && !block->isPrivateMethod() ) )
        && !block->isAbstract() )
   {
      addDefined(base);
   }
}






/*!
 * Implements _CppQt::Parse::Source_ interface. 
 *
 * @param block See interface docs. 
 */
void Header::evaluateOther(AbstractBlock* block)
{
   // If the given block is an enumeration then create an enumeration parser and 
   // append it to this object's declarations. 
   if ( CppQt::Enumeration* valid = block->cast<CppQt::Enumeration>(BlockFactory::EnumerationType) )
   {
      _declarations.append(new Enumeration(valid,this));
   }

   // Check to see if this object's namespace is a class. 
   if ( _block->type() == BlockFactory::ClassType )
   {
      // If the given block is an access type then create an access parser and append it 
      // to this object's declarations. 
      if ( CppQt::Access* valid = block->cast<CppQt::Access>(BlockFactory::AccessType) )
      {
         _declarations.append(new Access(valid,this));
      }

      // Else if the given block is a class then create a forward parser and append it 
      // to this object's declarations. 
      else if ( Class* valid = block->cast<Class>(BlockFactory::ClassType) )
      {
         _declarations.append(new Forward(valid,this));
      }
   }
}






/*!
 * Adds the preprocessor header guard lines to output. This does not include the 
 * ending statement to finish the guard which is done later. 
 */
void Header::outputGuard()
{
   // Add the first and then second preprocessor guard lines. 
   add(_guard1);
   add(_guard2);
}






/*!
 * Adds the declaration section of this parser to output. 
 */
void Header::outputDeclarations()
{
   // Make sure the declaration list is not empty. 
   if ( _declarations.isEmpty() ) return;

   // Get a pointer to this object's namespace as a class if it is a class, else this 
   // will be null. 
   const Class* block {_block->cast<const Class>(BlockFactory::ClassType)};

   // Check to see if this object's namespace is a class. 
   if ( block )
   {
      // Add the comments and declaration header of this object's class to output. 
      outputClassComments(block);
      outputClassDeclaration(block);

      // Add the opening class bracket to output and increase the indent spaces. 
      add("{");
      setIndent(indent() + _indentSpaces);

      // If this object's class is a qt object then add the appropriate macro to output. 
      if ( block->isQtObject() ) add("Q_OBJECT");
   }

   // Iterate through the list of all base declaration parser object's for this 
   // parser and add their output. 
   for (auto declaration : qAsConst(_declarations)) declaration->outputDeclaration();

   // Check to see if this object's namespace is a class again. 
   if ( block )
   {
      // Decrease the indent spaces and then add the closing class bracket to output. 
      setIndent(indent() - _indentSpaces);
      add("};");
   }
}






/*!
 * Adds the comments of the given class block to output. The given class is assumed 
 * to be this object's namespace which is a class. 
 *
 * @param block  
 */
void Header::outputClassComments(const Class* block)
{
   // Add the beginning comment line to output. 
   add("/*!");

   // Add the description comment to output, including template argument comments if 
   // the given class has any. 
   add(Base::makeComment(block->description()));
   if ( block->hasTemplates() ) add(Base::makeTemplateComments(_block));

   // Add the ending comment line to output. 
   add(" */");
}






/*!
 * Adds the class declaration header line to output using the given class block. 
 *
 * @param block  
 */
void Header::outputClassDeclaration(const Class* block)
{
   // Create the line starting with any possible template declaration, adding a space 
   // if there is one. 
   QString line {Base::makeTemplateDeclaration(block)};
   if ( !line.isEmpty() ) line.append(" ");

   // Append the class keyword and then the scoped class name to the line. 
   line.append("class ").append(Base::makePreClassScope(block->parent())).append(block->Base::name());

   // Iterate through all children of the given class block. 
   bool first {true};
   for (auto child : block->list())
   {
      // Make sure the child is a parent block type. 
      if ( Parent* valid = child->cast<Parent>(BlockFactory::ParentType) )
      {
         // Check to see if this is the first parent being added to the line. 
         if ( first )
         {
            // Append the beginning of parents marker to the line. 
            line.append(" : ");
            first = false;
         }

         // Else this is not the first to append a comma to the line. 
         else line.append(", ");

         // Append the full inheritance declaration for the child parent block to the line. 
         line.append(valid->accessString()).append(" ").append(valid->className());
      }
   }

   // Add the class declaration header line to output. 
   add(line);
}






/*!
 * Adds the closing preprocessor statement for the header guard. 
 */
void Header::outputFooter()
{
   // Add blank header lines and then the closing header guard preprocessor line to 
   // output. 
   add(_headerLines);
   add("#endif");
}
