#include "glsl_parse.h"
#include "glsl_parse_sourceparser.h"
#include "glsl_parse_commentparser.h"
#include "glsl_parse_functionparser.h"
#include "glsl_settings.h"
#include "glsl_factory.h"
#include "glsl_namespace.h"
#include "glsl_shader.h"
#include "glsl_variable.h"
#include "glsl_struct.h"
#include "glsl_function.h"
#include "basic_lineparser.h"
#include "scanner.h"
using LineParser = Basic::LineParser;



namespace GLSL
{
namespace Parse
{






/*!
 * Creates and returns a complete mapping of all scanner objects for the given
 * root block of a GLSL project.
 *
 * The parsing of a project is done through this function alone. Only one type
 * of source file is generated, however different sub extensions are made based
 * off the shader type of the source file. Fake namespace scope is also
 * prepended to each file name to allow for better organization of GLSL code.
 *
 * @param root The root block of a GLSL project used to generate all scanner
 *             objects and insert them into the given mapping to parse all
 *             possible files.
 *
 * @return Mapping of all scanner objects for the given root block.
 */
QMap<QString,Scanner*> createScannerMap(const Abstract::Block* root)
{
   // Make sure the given root pointer is valid.
   Q_CHECK_PTR(root);

   // Create the mapping that is returned.
   QMap<QString,Scanner*> ret;

   // Cast the given root pointer to a namespace block and make sure it worked.
   const Namespace* namespaceRoot {root->cast<const Namespace>(Factory::NamespaceType)};
   Q_CHECK_PTR(namespaceRoot);

   // Recursively add all source file scanner objects from the given root namespace.
   addSource(&ret,*namespaceRoot,QString());

   // Return the fully populated mapping of scanner objects.
   return ret;
}






/*!
 * Recursively adds all scanner objects to the given mapping from all children
 * shader objects of any depth from the given namespace. The given scope must be
 * the scope of the given namespace, not including itself, separated by
 * underscores and all lowercase. The scope is used to create the file name key
 * of inserted scanner objects.
 *
 * @param map The mapping that has all scanner objects of all child shader
 *            objects added to it.
 *
 * @param root The root namespace that is searched for any child namespace and
 *             shader blocks. This function is recursively called on namespace
 *             children to find all shader children of any depth.
 *
 * @param scope The scope of the given namespace, not including itself, all
 *              lowercase and separated by spaces.
 */
void addSource(QMap<QString,Scanner*>* map, const Namespace& root, QString scope)
{
   // Make sure the given mapping pointer is valid.
   Q_CHECK_PTR(map);

   // Iterate through all children of the given namespace block.
   for (auto child: root.list())
   {
      // If the child block is a namespace then recursively call this function on it,
      // adding the scope name of the given namespace.
      if ( const Namespace* valid = child->cast<const Namespace>(Factory::NamespaceType) )
      {
         if ( !scope.isEmpty() ) scope += QStringLiteral("_");
         addSource(map,*valid,scope + root.baseName().toLower());
      }

      // If the child block is a shader then add its scanner object, adding the scope
      // name of the given namespace.
      else if ( const Shader* valid = child->cast<const Shader>(Factory::ShaderType) )
      {
         if ( !scope.isEmpty() ) scope += QStringLiteral("_");
         addSource(map,*valid,scope + root.baseName().toLower());
      }
   }
}






/*!
 * Creates and adds the scanner object to the given mapping from the given
 * shader object, using the given scope to generate the file name key for
 * insertion.
 *
 * @param map The mapping which has the created scanner object added to it.
 *
 * @param shader The shader object used to create the scanner object that is
 *               added.
 *
 * @param scope The namespace scope of the given shader object, used to generate
 *              the file name key where the created scanner object is inserted.
 */
void addSource(QMap<QString,Scanner*>* map, const Shader& shader, QString scope)
{
   // Make sure the given mapping pointer is valid.
   Q_CHECK_PTR(map);

   // Grab all relevant global settings.
   const int variableLines {Settings::instance().variableLines()};
   const int structLines {Settings::instance().structLines()};
   const int functionLines {Settings::instance().functionLines()};

   // Create the scanner and add the source parser to it.
   Scanner* scanner {new Scanner};
   scanner->addParser(new SourceParser);

   // Iterate through all children of the given shader block.
   for (auto child: shader.list())
   {
      // Figure out the block type of the child.
      switch (child->type())
      {
      case Factory::VariableType:
         {
            // Cast the child as a variable and make sure it worked.
            const Variable* variable {child->cast<const Variable>(Factory::VariableType)};
            Q_CHECK_PTR(variable);

            // Add a variable lines number of blank lines to the scanner and then add the
            // variable child's definition to it.
            scanner->addParser(new LineParser(variableLines));
            add(scanner,*variable,0);
            break;
         }
      case Factory::StructType:
         {
            // Cast the child as a struct and make sure it worked.
            const Struct* variable {child->cast<const Struct>(Factory::StructType)};
            Q_CHECK_PTR(variable);

            // Add a struct lines number of blank lines to the scanner and then add the struct
            // child's definition to it.
            scanner->addParser(new LineParser(structLines));
            add(scanner,*variable);
            break;
         }
      case Factory::FunctionType:
         {
            // Cast the child as a function and make sure it worked.
            const Function* variable {child->cast<const Function>(Factory::FunctionType)};
            Q_CHECK_PTR(variable);

            // Add a function number of blank lines to the scanner and then add the function
            // child's definition to it.
            scanner->addParser(new LineParser(functionLines));
            add(scanner,*variable);
            break;
         }
      }
   }

   // Add the special main function definition to the scanner.
   scanner->addParser(new LineParser(functionLines));
   addMain(scanner,shader);

   // Add one final blank line to the end of the scanner and then insert it into the
   // given mapping.
   scanner->addParser(new LineParser(1));
   insert(map,scanner,shader,scope);
}






/*!
 * Inserts the given scanner object into the given mapping using the given
 * shader object and namespace scope to generate the file name key for
 * insertion.
 *
 * @param map The mapping which has the given scanner added to it.
 *
 * @param scanner The scanner object that is added to the given mapping.
 *
 * @param shader The shader object used to generate the file name key for
 *               insertion.
 *
 * @param scope The namespace scope of the given shader object used to generate
 *              the file name key for insertion.
 */
void insert(QMap<QString,Scanner*>* map, Scanner* scanner, const Shader& shader, QString scope)
{
   // Make sure the given pointers are valid.
   Q_CHECK_PTR(map);
   Q_CHECK_PTR(scanner);

   // Create the file name key, initializing it to the given scope and shader block's
   // base name separated by an underscore if the scope is not empty.
   if ( !scope.isEmpty() ) scope += QStringLiteral("_");
   QString fileName {scope + shader.baseName()};

   // Append the appropriate sub extension to the file name based off the given
   // shader block's type.
   switch (shader.shaderType())
   {
   case Shader::Vertex:
      fileName += QStringLiteral(".v");
      break;
   case Shader::TessellationControl:
      fileName += QStringLiteral(".tc");
      break;
   case Shader::TessellationEvaluation:
      fileName += QStringLiteral(".te");
      break;
   case Shader::Geometry:
      fileName += QStringLiteral(".g");
      break;
   case Shader::Fragment:
      fileName += QStringLiteral(".f");
      break;
   case Shader::Compute:
      fileName += QStringLiteral(".c");
      break;
   }

   // Add the GLSL extension to the end of the now complete file name key.
   fileName += QStringLiteral(".glsl");

   // Make sure the given mapping does not already contain a scanner object with the
   // file name key.
   if ( map->contains(fileName) )
   {
      qDebug().noquote() << QObject::tr("WARNING: scanner with file name key '%1' already exists, discarding duplicate scanner!")
                            .arg(fileName);
      delete scanner;
   }

   // Insert the given scanner object into the given mapping with the file name key
   // generated.
   map->insert(fileName,scanner);
}






/*!
 * Adds parser elements for the given variable block to the given scanner,
 * adding the given indent to each parser element created.
 *
 * @param scanner The scanner which has the created parser elements added to it.
 *
 * @param block The variable block used to create the parser elements defining
 *              it.
 *
 * @param indent The indent in spaces added to each parser element created.
 */
void add(Scanner* scanner, const Variable& block, int indent)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Create and add a comment parser to the given scanner, setting its indent to the
   // one given and adding the given variable's description.
   CommentParser* comment {new CommentParser};
   comment->setIndent(indent);
   comment->add(block.description());
   scanner->addParser(comment);

   // Create the definition string, initializing it with any layout the given
   // variable may contain and its type.
   QString definition {createLayout(block) + block.typeString()};

   // Add the given variable's name to the definition, separated by a space if it is
   // not empty.
   const QString name {block.baseName()};
   if ( !name.isEmpty() ) definition += QStringLiteral(" ");
   definition += name + QStringLiteral(";");

   // Create and add the definition line parser to the given scanner, setting its
   // indent to the one given.
   scanner->addParser(new LineParser(indent,definition));
}






/*!
 * Adds parser elements for the given struct block to the given scanner. This
 * includes all children variables contained within the struct block, indented
 * with the appropriate global setting.
 *
 * @param scanner The scanner which has the created parser elements added to it.
 *
 * @param block The struct block used to create the parser elements for itself
 *              and all its children variable blocks.
 */
void add(Scanner* scanner, const Struct& block)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Create and add the description comment block to the given scanner.
   CommentParser* comment {new CommentParser};
   comment->add(block.description());
   scanner->addParser(comment);

   // Create and add the opening definition line parser to the given scanner.
   LineParser* line {new LineParser(0,createLayout(block) + block.typeString())};
   line->add("{");
   scanner->addParser(line);

   // Iterate through all variable children of the given struct block, adding their
   // parser element definition to the given scanner indented withe the appropriate
   // global setting.
   for (auto variable: block.createListOfType<const Variable>(Factory::VariableType))
   {
      add(scanner,*variable,Settings::instance().indentSpaces());
   }

   // Create and add the closing definition line parser to the given scanner.
   line = new LineParser(0,QStringLiteral("}"));
   line->add(block.baseName() + QStringLiteral(";"));
   scanner->addParser(line);
}






/*!
 * Creates and returns the layout qualifiers for the given variable block.
 *
 * @param block The variable block used to create the layout string.
 *
 * @return Layout qualifiers for the given variable block.
 */
QString createLayout(const Variable& block)
{
   // Create a new string that will be returned as the created layout.
   QString ret;

   // If there are no qualifiers then return an empty string.
   const QStringList qualifiers {block.layout()};
   if ( qualifiers.isEmpty() ) return ret;

   // Add the beginning of the layout definition.
   ret += QStringLiteral("layout (");

   // Iterate through all qualifiers and add them to the layout definition.
   bool first {true};
   for (auto qualifier: qualifiers)
   {
      if ( !first ) ret += QStringLiteral(", ");
      else first = false;
      ret += qualifier;
   }

   // Add the now complete end of the layout definition and then return it.
   ret += QStringLiteral(") ");
   return ret;
}






/*!
 * Adds parser elements for the given function block to the given scanner.
 *
 * @param scanner The scanner which has the created parser elements added to it.
 *
 * @param block The function block used to create the parser elements defining
 *              it.
 */
void add(Scanner* scanner, const Function& block)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Create a comment parser and add the given function's description to it.
   CommentParser* comment {new CommentParser};
   comment->add(block.description());

   // Create the header string and initialize it with the given function's return
   // type, base name, and opening parenthesis.
   QString header
   {
      block.returnType() + QStringLiteral(" ") + block.baseName() + QStringLiteral("(")
   };

   // Iterate through all arguments of the given function.
   bool first {true};
   for (auto argument: block.arguments())
   {
      // If this is not the first argument then add a comma and space to the header.
      if ( first ) first = false;
      else header += QStringLiteral(", ");

      // Add the argument's definition to the header string.
      header += argument->typeString() + QStringLiteral(" ") + argument->baseName();

      // Add the argument's comment block to the comment parser.
      comment->add(1);
      comment->add(QStringLiteral("@param ") + argument->baseName(),argument->description());
   }

   // Add the comment parser to the given scanner and then finish the header string
   // with a closing parenthesis.
   scanner->addParser(comment);
   header += QStringLiteral(")");

   // Create a function parser with the completed header and the given function's
   // steps of operation.
   FunctionParser* function {new FunctionParser(header,block.operations())};

   // Add the given function's base name and open parenthesis to the header
   // expression.
   function->add(block.baseName() + QStringLiteral("("));

   // Iterate through all arguments of the given function again.
   first = true;
   for (auto argument: block.arguments())
   {
      // If this is not the first argument then add the comma separated expression.
      if ( first ) first = false;
      else function->addExp(QStringLiteral(",\\s*"));

      // Add the argument's expression for matching.
      function->add(argument->typeString());
      function->addExp(QStringLiteral("\\s+[a-zA-Z_]+[a-zA-Z_0-9]*\\s*"));
   }

   // Add the closing parenthesis to the header expression and then add the function
   // parser to the given scanner.
   function->add(QStringLiteral(")"));
   scanner->addParser(function);
}






/*!
 * Adds parser elements for the special main function of the given shader object
 * to the given scanner.
 *
 * @param scanner The scanner which has parser elements added to it.
 *
 * @param shader The shader object used to create the parser elements for the
 *               special main function.
 */
void addMain(Scanner* scanner, const Shader& shader)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Create and add the comment block for the special main function to the given
   // scanner, using the given shader block for the description.
   CommentParser* comment {new CommentParser};
   comment->add(shader.description());
   scanner->addParser(comment);

   // Create and add the function parser for the special main function to the given
   // scanner, using the given shader block for the steps of operation.
   FunctionParser* main {new FunctionParser(QStringLiteral("void main()"),shader.operations())};
   main->add(QStringLiteral("void main()"));
   scanner->addParser(main);
}

}
}
