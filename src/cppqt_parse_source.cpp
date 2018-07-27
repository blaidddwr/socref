#include "cppqt_parse_source.h"
#include <QStack>
#include <QRegularExpression>
#include "cppqt_parse_function.h"
#include "cppqt_parse_variable.h"
#include "cppqt_function.h"
#include "cppqt_variable.h"
#include "cppqt_namespace.h"
#include "cppqt_class.h"
#include "cppqt_access.h"
#include "cppqt_blockfactory.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 * Constructs a new source parser with the given namespace block and file name. 
 *
 * @param block The namespace block that is used to generate the parsing of this 
 *              parser object's file. This could also possibly be a class block. 
 *
 * @param name The file name that this object parses, excluding the extension. 
 */
Source::Source(const Namespace* block, const QString& name):
   Source(block)
{
   // Set the include statement used for including the header file of this source 
   // file. 
   _include = QString("#include \"%1.h\"").arg(name);
}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Source::initialize()
{
   evaluateAll();
}






/*!
 * Implements _AbstractParser_ interface. 
 *
 * @param line See interface docs. 
 *
 * @return See interface docs. 
 */
bool Source::readLine(const QString& line)
{
   // If this line is the beginning of comments then mark it for this parser. 
   if ( QRegularExpression("\\A\\s*\\/\\*!\\z").match(line).hasMatch() ) _inComments = true;

   // Else if this line is the ending of comments then mark it for this parser. 
   else if ( QRegularExpression("\\A\\s*\\*\\/\\z").match(line).hasMatch() ) _inComments = false;

   // Else check if the given line is not currently within comments. 
   else if ( !_inComments )
   {
      // If the given line is the indicator for being past the top of the file then mark 
      // it for this parser. 
      if ( line == QStringLiteral("//") ) _pastTop = true;

      // If the given line is not past the top then call the read top interface. 
      if ( !_pastTop ) readTop(line);

      // Check to see if the given line is the beginning of a function definition. 
      if ( QRegularExpression("\\A.*\\([a-zA-Z0-9_,<>:&\\* ]*\\):?[ constexp]*\\z").match(line).hasMatch() )
      {
         // Check to see if this function is defined in the namespace of this parser. 
         if ( Function* child = findDefined(line) )
         {
            // Set the function parser cut off and then step into it for read line processing. 
            child->setCutOff(line.indexOf(QRegularExpression("\\S")));
            stepIntoChild(child);
         }

         // Else this is an undefined function found in the source file. 
         else
         {
            // Add a new undefined function parser and then step into it for read line 
            // processing. 
            addUndefined(new Function(line,this));
            stepIntoChild(_undefined.back());
         }
      }
   }

   // Always return true so the entire file is parsed. 
   return true;
}






/*!
 * Implements _AbstractParser_ interface. 
 */
void Source::makeOutput()
{
   // Add the include statement for this source file's header to the output. 
   add(_include);

   // Add any additional preprocessor lines to the output. 
   outputPreProcessor();

   // Add the header section to the output. 
   outputHeader();

   // Add the default using namespace declaration for this object's namespace if it 
   // is not an empty string, adding blank header lines if the header section was 
   // empty. 
   if ( _header.isEmpty() ) add(_headerLines);
   if ( !_usingName.isEmpty() ) add(_usingName);

   // Add the end of top separator. 
   add("//");

   // Add all function definitions to the output. 
   outputDefinitions();
}






/*!
 * This interface is called for each line of the preprocessor and header sections 
 * of this object's parsed file. This can be used to remember custom information 
 * for output. The default implementation remembers any custom preprocessor 
 * statements and using namespace statements. 
 *
 * @param line The line from the preprocessor or header section of this object's 
 *             file. 
 */
void Source::readTop(const QString& line)
{
   // If the given line is a preprocessor statement and it is not the default include 
   // statement then add it as a custom preprocessor line. 
   if ( QRegularExpression("\\A#.*\\z").match(line).hasMatch() && line != _include )
   {
      addPreProcess(line);
   }

   // If the given line is a using namespace statement and it is not the default 
   // using statement then add it as a custom header line. 
   else if ( QRegularExpression("\\A\\s*using\\s+namespace\\s+[a-zA-Z_]+[a-zA-Z0-9_]*;\\s*\\z").match(line).hasMatch()
             && line != _usingName )
   {
      addHeader(line.trimmed());
   }
}






/*!
 * This interface is called to process all real children variable blocks of this 
 * object's namespace. This processing occurs before any parsing and is meant to 
 * add them to this parser for output. All blocks for evaluate interfaces are 
 * called in the same order they appear in their real children list. The default 
 * implementation adds them if they belong in the source file. 
 *
 * @param block The real child variable block of this object's namespace to be 
 *              evaluated. 
 */
void Source::evaluateVariable(CppQt::Variable* block)
{
   // If this object's namespace is not a template, the given variable block is 
   // static, and is not a constant expression then create a new variable parser and 
   // add it to this parser. 
   if ( !isTemplate() && block->isStatic() && !block->isConstExpr() )
   {
      addVariable(new Variable(block,this));
   }
}






/*!
 * This interface is called to process all real children function blocks of this 
 * object's namespace. All specialized functions such as constructors or signals 
 * are included. This processing occurs before any parsing and is meant to add them 
 * to this parser for output. All blocks for evaluate interfaces are called in the 
 * same order they appear in their real children list. The default implementation 
 * adds them if they belong in the source file. 
 *
 * @param block The real child function block of this object's namespace to be 
 *              evaluated. 
 */
void Source::evaluateFunction(CppQt::Function* block)
{
   // If the this object's namespace is not a template, the given function block is 
   // not abstract, is not default, is not a signal, and has no template arguments or 
   // is a private method then create a new function parser and add it to this 
   // parser. 
   if ( !isTemplate()
        && !block->isAbstract()
        && !block->isDefault()
        && block->type() != BlockFactory::SignalType
        && ( !block->hasTemplates() || block->isPrivateMethod() ) )
   {
      addDefined(new Function(block,this));
   }
}






/*!
 * This interface is called to process all real children blocks that are not 
 * variables or functions of this object's namespace. This processing occurs before 
 * any parsing and is meant to add them to this parser for output. All blocks for 
 * evaluate interfaces are called in the same order they appear in their real 
 * children list. The default implementation does nothing. 
 *
 * @param block The real child block that is not a variable or function of this 
 *              object's namespace to be evaluated. 
 */
void Source::evaluateOther(AbstractBlock* block)
{
   Q_UNUSED(block)
}






/*!
 * Constructs a new source parser with the given namespace block. This constructor 
 * is only meant to be used internally by this class or the header class that 
 * inherits from this one. 
 *
 * @param block The namespace or class block that is used to generate the parsing 
 *              of this parser object's file. 
 */
Source::Source(const Namespace* block):
   Global(block),
   _block(block),
   _headerLines(Settings::instance().headerLines()),
   _functionLines(Settings::instance().functionLines()),
   _children(block->realChildren())
{
   // If the given namespace block is a class then determine if it has any templates. 
   if ( const Class* valid = block->cast<Class>(BlockFactory::ClassType) )
   {
      _isTemplate = valid->hasAnyTemplates();
   }

   // Make the default using namespace declaration for the scope of this source 
   // file's variables and functions. 
   makeUsingName();
}






/*!
 * Adds all preprocessor lines to output. 
 */
void Source::outputPreProcessor()
{
   for (auto line : _preProcess) add(line);
}






/*!
 * Adds all header section lines to output.  
 */
void Source::outputHeader()
{
   // Make sure the header section is not empty. 
   if ( !_header.isEmpty() )
   {
      // Add blank header lines and then add all header statement lines. 
      add(_headerLines);
      for (auto line : _header) add(line);
   }
}






/*!
 * Adds all variable and function definitions to output. 
 */
void Source::outputDefinitions()
{
   // Check to see if there are any variable definitions to add. 
   if ( !_variables.isEmpty() )
   {
      // Add blank header lines to output. 
      add(_headerLines);

      // Iterate through all of this object's variable definitions. 
      for (auto variable : qAsConst(_variables))
      {
         // Add the comments and then the definition of the variable parser to output. 
         variable->outputComments();
         variable->outputDefinition();
      }
   }

   // Iterate through all defined function definitions. 
   for (auto function : _defined)
   {
      // Add blank function lines, then the function's comments, and then its definition 
      // to output. 
      add(_functionLines);
      function->outputComments();
      function->outputDefinition();
   }

   // Iterate through all undefined function definitions. 
   for (auto function : _undefined)
   {
      // Add blank function lines, then the function's comments, and then its definition 
      // to output. 
      add(_functionLines);
      function->outputComments();
      function->outputDefinition();
   }
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
Function* Source::findDefined(const QString& definition)
{
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






/*!
 * Returns a constant reference to the pointer list of all real children of this 
 * object's namespace block. 
 *
 * @return Constant reference to this object's namespace block's real children. 
 */
const QList<AbstractBlock*>& Source::children() const
{
   return _children;
}






/*!
 * Adds a preprocessor line to the preprocessor section of this parser that will be 
 * added to that section's output. 
 *
 * @param line The preprocessor line that is added. 
 */
void Source::addPreProcess(const QString& line)
{
   _preProcess << line;
}






/*!
 * Adds a header line to the header section of this parser that will be added to 
 * that section's output. 
 *
 * @param line The header line that is added. 
 */
void Source::addHeader(const QString& line)
{
   _header << line;
}






/*!
 * Adds a variable parser to this parser that will be used for the definition 
 * section output. This does not take ownership of the variable parser object. 
 *
 * @param parser Pointer to the variable parser added to this parser for its 
 *               definition section output. 
 */
void Source::addVariable(Variable* parser)
{
   _variables << parser;
}






/*!
 * Adds a defined function parser to this parser that will be used for the 
 * definition section output. This does not take ownership of the function parser 
 * object. 
 *
 * @param defined Pointer to the defined function parser added to this parser for 
 *                its definition section output. 
 */
void Source::addDefined(Function* defined)
{
   _defined << defined;
}






/*!
 * Adds an undefined function parser to this parser that will be used for the 
 * definition section output. This does not take ownership of the function parser 
 * object. 
 *
 * @param undefined Pointer to the undefined function parser added to this parser 
 *                  for its definition section output. 
 */
void Source::addUndefined(Function* undefined)
{
   _undefined << undefined;
}






/*!
 * Tests if the namespace of this parser has any templates in itself or any parent 
 * class blocks. If it does this obviously means it is a class. 
 *
 * @return True if the namespace of this parser contains any templates or false 
 *         otherwise. 
 */
bool Source::isTemplate() const
{
   return _isTemplate;
}






/*!
 * Makes the default using namespace declaration for this source file. 
 */
void Source::makeUsingName()
{
   // Initialize a namespace pointer stack. 
   QStack<Namespace*> list;

   // Iterate up the parent blocks of this object's namespace block, starting with 
   // its parent. 
   AbstractBlock* back {_block->parent()};
   while ( back )
   {
      // If the current parent is a namespace block then push its pointer to the stack. 
      if ( Namespace* valid = back->cast<Namespace>(BlockFactory::NamespaceType) )
      {
         list.push(valid);
      }

      // Iterate up to the next parent. 
      back = back->parent();
   }

   // Make sure the stack has more than one pointer. If it only has one pointer that 
   // means the namespace of this parser is in the global namespace and no default 
   // using statement is required. 
   if ( list.size() > 1 )
   {
      // Pop the root namespace pointer from the stack. 
      list.pop();

      // Append the beginning of the using declaration, then all found namespaces in the 
      // stack, and then this object's namespace. 
      _usingName.append("using namespace ");
      while ( list.size() > 1 ) _usingName.append(list.pop()->Base::name()).append("::");
      _usingName.append(list.pop()->Base::name()).append(";");
   }
}






/*!
 * Evaluates all real children of this object's namespace, calling the appropriate 
 * evaluate interface on each one. 
 */
void Source::evaluateAll()
{
   // Iterate through all real children of this object's namespace. 
   for (auto child : qAsConst(_children))
   {
      // If the child is a variable block then call its evaluate interface. 
      if ( CppQt::Variable* valid = child->cast<CppQt::Variable>(BlockFactory::VariableType) )
      {
         evaluateVariable(valid);
      }

      // Else if the child is any type of function block then call its evaluate 
      // interface. 
      else if ( CppQt::Function* valid = qobject_cast<CppQt::Function*>(child) )
      {
         evaluateFunction(valid);
      }

      // Else this is another type of block so call its evaluate interface. 
      else evaluateOther(child);
   }
}
