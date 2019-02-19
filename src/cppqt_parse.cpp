#include "cppqt_parse.h"
#include "cppqt_parse_header.h"
#include "cppqt_parse_source.h"
#include "cppqt_parse_comment.h"
#include "cppqt_parse_function.h"
#include "cppqt_settings.h"
#include "cppqt_factory.h"
#include "cppqt_namespace.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"
#include "cppqt_class.h"
#include "cppqt_parent.h"
#include "cppqt_access.h"
#include "cppqt_function.h"
#include "cppqt_declaration.h"
#include "basic_lineparser.h"
#include "scanner.h"
using LineParser = Basic::LineParser;



namespace CppQt
{
namespace Parse
{






/*!
 * Creates and returns a complete mapping of all scanner objects for the given
 * root block of a C++/Qt project.
 *
 * The parsing of a project is done through this function alone. The four basic
 * types of source files generated for this project type are namespace headers,
 * class headers, namespace sources, and class sources. A fifth special and
 * unique source file is also created for the special main function of a C++
 * application.
 *
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

   // Add the special main source file scanner.
   addMain(&ret,*namespaceRoot);

   // Recursively add all header and source file scanner objects from the given root
   // namespace.
   addHeader(&ret,*namespaceRoot);
   addSource(&ret,*namespaceRoot);

   // Return the fully populated mapping of scanner objects.
   return ret;
}






/*!
 * Adds the special main source file scanner object to the given mapping,
 * searching for a main function in the given namespace root. If the special
 * main key already exists or no main function block is found then this does
 * nothing.
 *
 * @param map The mapping that possibly has the special main source file scanner
 *            object added to it.
 *
 * @param root The root namespace whose list of functions is searched for the
 *             special main function.
 */
void addMain(QMap<QString,Scanner*>* map, const Namespace& root)
{
   // Make sure the given mapping pointer is valid and it does not already contain
   // the special main source file key.
   Q_CHECK_PTR(map);
   if ( map->contains(QStringLiteral("main.cpp")) ) return;

   // Grab the global function lines setting.
   const int functionLines {Settings::instance().functionLines()};

   // Iterate through all function block children of the given namespace block.
   for (auto function: root.createListOfType<const CppQt::Function>(Factory::FunctionType))
   {
      // Check to see if the function's base name is main.
      if ( function->baseName() == QStringLiteral("main") )
      {
         // Create a new scanner object that is the main source file.
         Scanner* scanner {new Scanner};

         // Add the source parser and then a number of blank lines equal to the function
         // lines setting.
         scanner->addParser(new Source(true));
         scanner->addParser(new LineParser(functionLines));

         // Add the function definition parser elements to the scanner.
         QList<Abstract::Parser*> lines;
         addDefinition(&lines,*function);
         for (auto line: lines) scanner->addParser(line);

         // Add a final blank line to the scanner.
         scanner->addParser(new LineParser(1));

         // Insert the special main source file scanner object to the given mapping and
         // return because the unique function was found.
         map->insert(QStringLiteral("main.cpp"),scanner);
         return;
      }
   }
}






/*!
 * Adds a namespace header file scanner object to the given mapping using the
 * given namespace block, recursively adding all other children namespace and
 * class block's header file scanner objects.
 *
 * @param map The mapping that has all header file scanner objects added to it
 *            using the given namespace block and all its children.
 *
 * @param root The namespace block used to generate the scanner object and
 *             recursively add all children's scanners.
 */
void addHeader(QMap<QString,Scanner*>* map, const Namespace& root)
{
   // Make sure the given mapping pointer is valid.
   Q_CHECK_PTR(map);

   // Grab relevant global settings, create the scanner object to be added, and get
   // the namespace scope of the given namespace block.
   const int indentSpaces {Settings::instance().indentSpaces()};
   Scanner* scanner {new Scanner};
   QList<const Namespace*> scope {createScope(root)};

   // Add the header parser elements to the scanner.
   addHeader(scanner,scope);

   // Create the list of declaration, variable definition, and function definition
   // parser elements.
   QList<Abstract::Parser*> declarations;
   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createParsers(&declarations,&variables,&functions,root,scope.size()*indentSpaces,true);

   // Add all declaration, variable definition, and function definition parser
   // elements to the scanner.
   addParsers(scanner,&declarations,&variables,&functions,&scope);

   // Add the footer parser elements to the scanner.
   addFooter(scanner);

   // Insert the generated scanner object into the given mapping using the header
   // file extension.
   insertScanner(map,scanner,scope,QStringLiteral(".h"));

   // Iterate through all children blocks of the given namespace block.
   for (auto child: root.list())
   {
      // If the child is a namespace then recursively call this function on the child to
      // add its scanner object.
      if ( const Namespace* valid = child->cast<const Namespace>(Factory::NamespaceType) )
      {
         addHeader(map,*valid);
      }

      // Else if the child is a class then add its scanner object to the given mapping.
      else if ( const Class* valid = child->cast<const Class>(Factory::ClassType) )
      {
         addHeader(map,*valid);
      }
   }
}






/*!
 * Adds a class header file scanner object to the given mapping using the given
 * class block.
 *
 * @param map The mapping that has the header file scanner object added to it.
 *
 * @param root The class block used to generate the added scanner object.
 */
void addHeader(QMap<QString,Scanner*>* map, const Class& root)
{
   // Make sure the given mapping pointer is valid.
   Q_CHECK_PTR(map);

   // Grab the relevant global settings, create the scanner object to be added, and
   // get the namespace scope of the given class block.
   const int indentSpaces {Settings::instance().indentSpaces()};
   Scanner* scanner {new Scanner};
   QList<const Namespace*> scope {createScope(root)};

   // Add the header parser elements to the scanner.
   addHeader(scanner,scope,&root);

   // Create the list of declaration, variable definition, and function definition
   // parser elements.
   QList<Abstract::Parser*> declarations;
   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createClassParsers(&declarations,&variables,&functions,root,scope.size()*indentSpaces,true);

   // Add all declaration, variable definition, and function definition parser
   // elements to the scanner.
   addParsers(scanner,&declarations,&variables,&functions,&scope);

   // Add the footer parser elements to the scanner.
   addFooter(scanner);

   // Insert the generated scanner object into the given mapping using the header
   // file extension.
   insertScanner(map,scanner,scope,QStringLiteral(".h"),&root);
}






/*!
 * Adds a namespace source file scanner object to the given mapping using the
 * given namespace block, recursively adding all other children namespace and
 * class block's source file scanner objects.
 *
 * @param map The mapping that has all header file scanner objects added to it
 *            using the given namespace block and all its children.
 *
 * @param root The namespace block used to generate the scanner and recursively
 *             add all children's scanners.
 */
void addSource(QMap<QString,Scanner*>* map, const Namespace& root)
{
   // Make sure the given mapping pointer is valid.
   Q_CHECK_PTR(map);

   // Grab relevant global settings, create the scanner that is added, and create the
   // namespace scope of the given namespace.
   const int headerLines {Settings::instance().headerLines()};
   Scanner* scanner {new Scanner};
   QList<const Namespace*> scope {createScope(root)};

   // Add the header parser elements to the scanner.
   addSourceHeader(scanner,scope);

   // If the scope is not empty add a header lines number of blank spaces and then
   // the scope without adding to the indent.
   int indent {0};
   if ( !scope.isEmpty() ) scanner->addParser(new LineParser(headerLines));
   addScope(scanner,&indent,scope,true);

   // Create the list of variable and function definition parser elements.
   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createParsers(nullptr,&variables,&functions,root,0,false);

   // Add all variable and function definition parser elements to the scanner.
   addParsers(scanner,nullptr,&variables,&functions);

   // Add the footer parser elements to the scanner.
   scanner->addParser(new LineParser(1));
   endScope(scanner,&indent,true);
   scanner->addParser(new LineParser(1));

   // Insert the generated scanner object into the given mapping using the source
   // extension.
   insertScanner(map,scanner,scope,QStringLiteral(".cpp"));

   // Iterate through all children blocks of the given namespace.
   for (auto child: root.list())
   {
      // If the child is a namespace then recursively call this function to add its
      // scanner object and all its children's scanners.
      if ( const Namespace* valid = child->cast<const Namespace>(Factory::NamespaceType) )
      {
         addSource(map,*valid);
      }

      // If the child is a class then add its source file scanner object to the given
      // mapping.
      else if ( const Class* valid = child->cast<const Class>(Factory::ClassType) )
      {
         addSource(map,*valid);
      }
   }
}






/*!
 * Adds a source file scanner object to the given mapping using the given class
 * block.
 *
 * @param map The mapping that has the source file scanner object added to it.
 *
 * @param root The class block used to generate the added scanner object.
 */
void addSource(QMap<QString,Scanner*>* map, const Class& root)
{
   // Make sure the given mapping pointer is valid.
   Q_CHECK_PTR(map);

   // Grab the relevant global settings, create the scanner to be added, and create
   // the namespace scope of the given class block.
   const int headerLines {Settings::instance().headerLines()};
   Scanner* scanner {new Scanner};
   QList<const Namespace*> scope {createScope(root)};

   // Add the header parser elements to the scanner.
   addSourceHeader(scanner,scope,&root);

   // If the scope is not empty then add a header lines number of blank lines and
   // then the scope without adding to the indent.
   int indent {0};
   if ( !scope.isEmpty() ) scanner->addParser(new LineParser(headerLines));
   addScope(scanner,&indent,scope,true);

   // Create the list of variable and function definition parser elements.
   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createClassParsers(nullptr,&variables,&functions,root,0,false);

   // Add all variable and function definition parser elements to the scanner.
   addParsers(scanner,nullptr,&variables,&functions);

   // Add the footer parser elements to the scanner.
   scanner->addParser(new LineParser(1));
   endScope(scanner,&indent,true);
   scanner->addParser(new LineParser(1));

   // Insert the generated scanner object into the given mapping using the source
   // extension.
   insertScanner(map,scanner,scope,QStringLiteral(".cpp"),&root);
}






/*!
 * Adds the given declaration, variable definition, and function definition
 * lists of parser elements to the given scanner. Any given list pointer
 * argument can be null, in which case that list type is ignored. An optional
 * namespace scope list pointer can also be given, in which case each parser
 * list will be enclosed with opening and closing scope parser elements.
 *
 * @param scanner The scanner that has all given parser element lists added to
 *                it.
 *
 * @param declarations Optional list of declaration parser elements. These are
 *                     added to the given scanner before any other list. Set to
 *                     null to ignore this list.
 *
 * @param variables Optional list of variable definition parser elements. These
 *                  are added to the given scanner before the function
 *                  definition list and after the declaration list. Set to null
 *                  to ignore this list.
 *
 * @param functions Optional list of function definition parser elements. These
 *                  are added to the given scanner after any other list. Set to
 *                  null to ignore this list.
 *
 * @param scope Optional pointer to the namespace scope that each parser element
 *              list is enclosed by opening and closing scope parser elements.
 */
void addParsers(Scanner* scanner, QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, QList<const Namespace*>* scope)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Grab any relevant global settings.
   const int headerLines {Settings::instance().headerLines()};
   const int functionLines {Settings::instance().functionLines()};

   // Check to see if there is a declaration list and it is not empty.
   int indent {0};
   if ( declarations && !declarations->isEmpty() )
   {
      // Add a header lines number of blank lines to the given scanner.
      scanner->addParser(new LineParser(headerLines));

      // Add all declaration lines to given the scanner, enclosing them with opening and
      // closing scope parser elements if a scope is given.
      if ( scope ) addScope(scanner,&indent,*scope,false,true);
      for (auto parser: *declarations) scanner->addParser(parser);
      if ( scope ) endScope(scanner,&indent);
   }

   // Check to see if there is a variable definition list and it is not empty.
   if ( variables && !variables->isEmpty() )
   {
      // Add a header lines number of blank lines to the given scanner.
      scanner->addParser(new LineParser(headerLines));

      // Add all variable definition lines to the given scanner, enclosing them with
      // opening and closing scope parser elements and setting the correct indent if a
      // scope is given.
      if ( scope ) addScope(scanner,&indent,*scope);
      for (auto parser: *variables)
      {
         parser->setIndent(indent);
         scanner->addParser(parser);
      }
      if ( scope ) endScope(scanner,&indent);
   }

   // Check to see if there is a function definition list and it is not empty.
   if ( functions && !functions->isEmpty() )
   {
      // Add a function lines number of blank lines to the given scanner.
      scanner->addParser(new LineParser(functionLines));

      // Add all function definition lines to the given scanner, enclosing them with
      // opening and closing scope parser elements and setting the correct indent if a
      // scope is given.
      if ( scope ) addScope(scanner,&indent,*scope);
      for (auto parser: *functions)
      {
         parser->setIndent(indent);
         scanner->addParser(parser);
      }
      if ( scope ) endScope(scanner,&indent);
   }
}






/*!
 * Adds parser elements to the given scanner that constitute the header of a
 * header file for a namespace or class if one is given, using the given scope
 * for the file header.
 *
 * @param scanner The scanner which has header parser elements added to it.
 *
 * @param scope The namespace scope of the header file.
 *
 * @param name The class of the header file if it is for a class or null if it
 *             is for a namespace.
 */
void addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Create the header scope string used for the generation of the macro header
   // guard.
   QString headerScope;

   // Append each namespace name in the given scope to the header scope, making them
   // all uppercase and separated by underscores.
   bool first {true};
   for (auto name: scope)
   {
      if ( first ) first = false;
      else headerScope += QStringLiteral("_");
      headerScope += name->baseName().toUpper();
   }

   // If a class is provided then append its all uppercase name to the header scope.
   if ( name )
   {
      if ( !first ) headerScope += QStringLiteral("_");
      headerScope += name->baseName().toUpper();
   }

   // If the header scope is still empty then this is the root namespace so append
   // the special global name.
   if ( headerScope.isEmpty() ) headerScope += QStringLiteral("GLOBAL");

   // Add the two header guard macro lines to the given scanner.
   LineParser* line {new LineParser};
   line->add(QStringLiteral("#ifndef ") + headerScope + QStringLiteral("_H"));
   line->add(QStringLiteral("#define ") + headerScope + QStringLiteral("_H"));
   scanner->addParser(line);

   // Add a new header parser to the given scanner.
   scanner->addParser(new Header);
}






/*!
 * Adds parser elements to the given scanner that constitute the header of a
 * source file for a namespace or class if one is given, using the given scope
 * for the file header.
 *
 * @param scanner The scanner which has header parser elements added to it.
 *
 * @param scope The namespace scope of the source file.
 *
 * @param name The class of the source file if it is for a class or null if it
 *             is for a namespace.
 */
void addSourceHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name)
{
   // Make sure the given scanner pointer is valid.
   Q_CHECK_PTR(scanner);

   // Create the header scope string used for the generation of the special include
   // macro.
   QString headerScope;

   // Append each namespace name in the given scope to the header scope, making them
   // all lowercase and separated by underscores.
   bool first {true};
   for (auto name: scope)
   {
      if ( first ) first = false;
      else headerScope += QStringLiteral("_");
      headerScope += name->baseName().toLower();
   }

   // If a class is provided then append its all lowercase name to the header scope.
   if ( name )
   {
      if ( !first ) headerScope += QStringLiteral("_");
      headerScope += name->baseName().toLower();
   }

   // If the header scope is still empty then this is the root namespace so append
   // the special global name.
   if ( headerScope.isEmpty() ) headerScope += QStringLiteral("global");

   // Add the special macro include line and then a new source parser to the given
   // scanner.
   scanner->addParser(new LineParser(0
                                     ,QStringLiteral("#include \"")
                                      + headerScope
                                      + QStringLiteral(".h\"")));
   scanner->addParser(new Source);
}






/*!
 * Adds parser elements to the given scanner that constitute the footer of a
 * header file.
 *
 * @param scanner The scanner that has footer parser elements added to it.
 */
void addFooter(Scanner* scanner)
{
   // Add the footer end of the macro header guard of a header file to the given
   // scanner.
   LineParser* line {new LineParser};
   line->add(1);
   line->add(QStringLiteral("#endif"));
   line->add(1);
   scanner->addParser(line);
}






/*!
 * Inserts the given scanner into the given mapping. The given namespace scope,
 * ending, and optional class are used to determine the filename key used for
 * insertion. The filename is generated with the namespace scope, then the
 * optional class name if it is given, and then the ending string.
 *
 * @param map The mapping that has the given scanner inserted into it.
 *
 * @param scanner The scanner object that is inserted into the given mapping.
 *
 * @param scope The namespace scope used to generate the insertion's filename
 *              key.
 *
 * @param ending The ending string used to generate the insertion's filename
 *               key.
 *
 * @param name The optional class block scope used to generate the insertion's
 *             filename key.
 */
void insertScanner(QMap<QString,Scanner*>* map, Scanner* scanner, const QList<const Namespace*>& scope, const QString& ending, const Class* name)
{
   // Create the file name string.
   QString fileName;

   // Append each namespace name of the given scope to the file name, making them all
   // lowercase and separated by underscores.
   bool first {true};
   for (auto name: scope)
   {
      if ( first ) first = false;
      else fileName += QStringLiteral("_");
      fileName += name->baseName().toLower();
   }

   // If a class is provided then append its all lowercase name to the file name.
   if ( name )
   {
      if ( !first ) fileName += QStringLiteral("_");
      fileName += name->baseName().toLower();
   }

   // If the file name is still empty then this is the root namespace so append the
   // special global name.
   if ( fileName.isEmpty() ) fileName += QStringLiteral("global");

   // Append the given ending to the file name.
   fileName += ending;

   // Make sure the given mapping does not already contain a scanner object with the
   // given file name key, deleting the given scanner object and warning the user if
   // it does.
   if ( map->contains(fileName) )
   {
      qDebug().noquote() << QObject::tr("WARNING: scanner with file name key '%1' already exists, discarding duplicate scanner!")
                            .arg(fileName);
      delete scanner;
   }

   // Insert the given scanner object into the given mapping with the generated file
   // name.
   else map->insert(fileName,scanner);
}






/*!
 * Creates and returns a list of namespace pointers that is the scope of the
 * given block. This includes the block itself if it is a namespace.
 *
 * @param block The block whose namespace scope list is created and returned.
 *
 * @return List of namespace pointers that is the scope of the given block.
 */
QList<const Namespace*> createScope(const Abstract::Block& block)
{
   // Create the namespace pointer list that will be the scope list returned.
   QList<const Namespace*> ret;

   // Iterate up the list of parent blocks starting at the given block until the root
   // namespace is reached.
   const Abstract::Block* up {&block};
   while ( up->parent() )
   {
      // If the block is a namespace then prepend its pointer to the scope list.
      if ( const Namespace* valid = up->cast<Namespace>(Factory::NamespaceType) )
      {
         ret.prepend(valid);
      }

      // Move up to the next block parent.
      up = up->parent();
   }

   // Return the generated namespace scope list.
   return ret;
}






/*!
 * Adds line parser elements to the given scanner that is the opening namespace
 * scope of the given scope, adding to the given indent for each namespace
 * scoped. Additional optional arguments are given for keeping the lines flat
 * and not adding indent spaces and describing each namespace opened.
 *
 * @param scanner The scanner that has line parser elements added to it that is
 *                the opening namespace scope of the one given.
 *
 * @param indent Pointer to the indent that is incremented for each namespace in
 *               the scope given. It is incremented by the global indent spaces
 *               value for each namespace in the given scope.
 *
 * @param scope The namespace scope used to generate the opening scope lien
 *              parser elements.
 *
 * @param flat True to not add any indent spaces for each consecutive namespace
 *             in the provided scope or false to add indent spaces. This does
 *             NOT effect the provided indent integer which is incremented
 *             regardless.
 *
 * @param describe True to add comments for each opening namespace added or
 *                 false to not add any comments.
 */
void addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope, bool flat, bool describe)
{
   // Make sure the given scanner and indent pointers are valid.
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);

   // Iterate through every namespace pointer of the given scope list.
   for (auto name: scope)
   {
      // If describe is enabled then add a comment parser for the namespace to the given
      // scanner, setting its indent if flat is disabled.
      if ( describe )
      {
         Comment* comment {new Comment};
         if ( !flat ) comment->setIndent(*indent);
         comment->add(name->description());
         scanner->addParser(comment);
      }

      // Add the opening bracket line parser for the namespace to the given scanner,
      // setting its indent if flat is disabled.
      LineParser* line
      {
         new LineParser(0,QStringLiteral("namespace ") + name->baseName())
      };
      if ( !flat ) line->setIndent(*indent);
      line->add(QStringLiteral("{"));
      scanner->addParser(line);

      // Increment the given indent value by the global indent spaces setting.
      *indent += Settings::instance().indentSpaces();
   }
}






/*!
 * Adds closing bracket line parser elements to the given scanner, using the
 * given indent to determine how many closing brackets are added. An optional
 * option is provided for not adding indent spaces to each line parser.
 *
 * @param scanner The scanner that has closing bracket line parser elements
 *                added to it.
 *
 * @param indent The indent value that determines how many closing brackets are
 *               generated. For each closing bracket this value is decremented
 *               by the global indent spaces setting until it is zero or less.
 *
 * @param flat True to not add any indent spaces for each consecutive closing
 *             bracket or false to add indent spaces.
 */
void endScope(Scanner* scanner, int* indent, bool flat)
{
   // Make sure the given scanner and indent pointers are valid.
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);

   // Continue while the indent value is greater than zero.
   while ( *indent > 0 )
   {
      // Decrement the indent value by the global indent spaces setting and then add a
      // closing bracket line parser, setting its indent by the indent value if flat is
      // disabled.
      *indent -= Settings::instance().indentSpaces();
      scanner->addParser(new LineParser(flat? 0 : *indent,QStringLiteral("}")));
   }
}






/*!
 * Creates all parser elements from the given block for the optionally given
 * declarations, variable definitions, and function definitions. If a parser
 * list is null then that type of parser element is ignored and never created.
 * If the given block is a class then all child classes have their parser
 * elements recursively added to the declarations if it is not null with the
 * correct indent added to them all. The indent is NOT set for variable and
 * function definitions. The header flag is used to determine what variable and
 * function blocks are added as definitions if those lists are provided.
 *
 * This is one of two primary functions that can be considered the core or
 * engine of this project type's parsing functions. This, along with the other
 * class function, creates all parser elements for files.
 *
 * @param declarations Declarations list that all declaration parser elements
 *                     are added to if this is not null. If this is null then
 *                     all declarations are ignored.
 *
 * @param variables Variable definitions list that all variable definition
 *                  parser elements are added to if this is not null. If this is
 *                  null then all variable definitions are ignored.
 *
 * @param functions Function definitions list that all function definition
 *                  parser elements are added to if this is not null. If this is
 *                  null then all function definitions are ignored.
 *
 * @param block The block used to generate all parser elements of the provided
 *              list types.
 *
 * @param indent The indent in spaces to start with for declaration parser
 *               elements. This is ignored for definition lists.
 *
 * @param isHeader True if the given lists are meant for a header file or false
 *                 if they are meant for a source file.
 */
void createParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Abstract::Block& block, int indent, bool isHeader)
{
   // Grab relevant global settings.
   const int functionLines {Settings::instance().functionLines()};

   // Iterate through all children of the given block.
   for (auto child: block.list())
   {
      // If the child block is an enumeration and a declarations list is given then add
      // its declaration parser elements to the list.
      if ( const Enumeration* valid = child->cast<const Enumeration>(Factory::EnumerationType) )
      {
         if ( declarations ) add(declarations,*valid,indent);
      }

      // Else check if the child block is a class.
      else if ( const Class* valid = child->cast<const Class>(Factory::ClassType) )
      {
         // If the child class is part of a parent class then create all of its parser
         // elements with the provided lists.
         if ( block.type() == Factory::AccessType )
         {
            createClassParsers(declarations,variables,functions,*valid,indent,isHeader);
         }

         // Else if this is a root class part of a namespace, a declarations list is
         // provided, and it has no templates then add its forward declaration to the list.
         else if ( declarations && !valid->hasAnyTemplates() )
         {
            *declarations << new LineParser(indent
                                            ,QStringLiteral("class ")
                                             + valid->baseName()
                                             + QStringLiteral(";"));
         }
      }

      // Else check if the child block is a variable.
      else if ( const Variable* valid = child->cast<const Variable>(Factory::VariableType) )
      {
         // If a declaration list is provided then add the child variable declaration to
         // the list.
         if ( declarations ) addDeclaration(declarations,*valid,indent);

         // If a variable definition list is provided and the child variable block has a
         // definition for this file type then add its definition to the list.
         if ( variables && hasDefinition(*valid,isHeader) ) addDefinition(variables,*valid);
      }

      // Else check if the child block is a function.
      else if ( const CppQt::Function* valid = child->cast<const CppQt::Function>(Factory::FunctionType) )
      {
         // If a declaration list is provided then add the child function's declaration to
         // the list.
         if ( declarations )
         {
            addDeclaration(declarations,*valid,indent);
         }

         // Check if a function definition list is provided and the child function has a
         // definition for this type type.
         if ( functions && hasDefinition(*valid,isHeader) )
         {
            // Add the child function's definition to the list, adding a function lines number
            // of blank lines to the list beforehand if this is not the first definition to be
            // added.
            if ( !functions->isEmpty() ) *functions << new LineParser(functionLines);
            addDefinition(functions,*valid);
         }
      }

      // Else If the child block is a declaration and a declaration list is provided
      // then add its line to the list.
      else if ( const Declaration* valid = child->cast<const Declaration>(Factory::DeclarationType) )
      {
         if ( declarations )
         {
            *declarations << new LineParser(indent,valid->line() + QStringLiteral(";"));
         }
      }
   }
}






/*!
 * Creates all parser elements from the given class block for the optionally
 * provided declarations, variable definitions, and function definitions. If a
 * parser list is null then that type of parser element is ignored and never
 * created. The given indent is only added to declaration parser elements and
 * ignored otherwise. The header flag is used to determine what variable and
 * function blocks are added as definitions is those lists are provided.
 *
 * This is one of two primary functions that can be considered the core or
 * engine of this project type's parsing functions. This, along with the other
 * base function, creates all parser elements for files.
 *
 * @param declarations Declarations list that all declaration parser elements
 *                     are added to if this is not null. If this is null then
 *                     all declarations are ignored.
 *
 * @param variables Variable definitions list that all variable definition
 *                  parser elements are added to if this is not null. If this is
 *                  null then all variable definitions are ignored.
 *
 * @param functions Function definitions list that all function definition
 *                  parser elements are added to if this is not null. If this is
 *                  null then all function definitions are ignored.
 *
 * @param root The class block used to generate all parser elements of the
 *             provided list types.
 *
 * @param indent The indent in spaces to start with for declaration parser
 *               elements. This is ignored for definition lists.
 *
 * @param isHeader True if the given lists are meant for a header file or false
 *                 if they are meant for a source file.
 */
void createClassParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Class& root, int indent, bool isHeader)
{
   // Grab relevant global settings.
   const int indentSpaces {Settings::instance().indentSpaces()};

   // Check if a declarations list is provided.
   if ( declarations )
   {
      // Create a new comment for the given class, adding its description to it and
      // setting its indent to the one given.
      Comment* comment {new Comment};
      comment->setIndent(indent);
      comment->add(root.description());

      // Iterate through all templates arguments for the given class, adding a comment
      // description for each template separated by one blank comment line.
      for (auto temp: root.templates())
      {
         comment->add(1);
         comment->add(QStringLiteral("@tparam ") + temp->baseName(),temp->description());
      }

      // Add the completed comment parser element to the given declaration list.
      *declarations << comment;

      // Create the header string initialized with the class name declaration.
      QString header {QStringLiteral("class ") + root.baseName()};

      // Get the given class block's list of parent blocks and make sure it is not
      // empty.
      QList<const Parent*> parents {root.createListOfType<const Parent>(Factory::ParentType)};
      if ( !parents.isEmpty() )
      {
         // Append the beginning of parents declaration to the header.
         header += QStringLiteral(" : ");

         // Iterate through all parent blocks, adding each one's declaration to the header
         // separated by commas and a space.
         bool first {true};
         for (auto name: parents)
         {
            if ( first ) first = false;
            else header += QStringLiteral(", ");
            header += name->accessString() + QStringLiteral(" ") + name->className();
         }
      }

      // Create a new line parser with the given indent and completed header string,
      // followed by an opening bracket line, and then add it to the given declaration
      // list.
      LineParser* line {new LineParser(indent,header)};
      line->add(QStringLiteral("{"));
      *declarations << line;

      // If the given class is a qt object then add the appropriate macro line parser to
      // the given declaration list.
      if ( root.isQtObject() )
      {
         *declarations << new LineParser(indent + indentSpaces,QStringLiteral("Q_OBJECT"));
      }
   }

   // Iterate through all access block children of the given class block.
   for (auto access: root.createListOfType<const Access>(Factory::AccessType))
   {
      // If a declaration list is provided then add the access declaration line parser
      // to the list.
      if ( declarations ) *declarations << new LineParser(indent,access->accessString());

      // Create all parser elements for the provided lists from the access block, adding
      // an indent spaces number of spaces to the indent.
      createParsers(declarations,variables,functions,*access,indent + indentSpaces,isHeader);
   }

   // If a declaration list is provided then add the closing class bracket line
   // parser to it.
   if ( declarations ) *declarations << new LineParser(indent,QStringLiteral("};"));
}






/*!
 * Adds declaration parser elements for the given enumeration to the given
 * declaration list using the given indent.
 *
 * @param list The declaration list that has the created parser elements of the
 *             given enumeration added to it.
 *
 * @param enumeration The enumeration block used to generate the declaration
 *                    parser elements.
 *
 * @param indent The indent in spaces added to all declaration parser elements
 *               created.
 */
void add(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent)
{
   // Make sure the given list pointer is valid.
   Q_CHECK_PTR(list);

   // Create a comment parser with the given enumeration's description, setting its
   // indent to the one given and adding it to he given list.
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(enumeration.description());
   *list << comment;

   // Create the opening enumeration declaration line parser for the given
   // enumeration block, setting its indent to the one given and adding it to the
   // given list.
   LineParser* line {new LineParser};
   line->setIndent(indent);
   QString header {QStringLiteral("enum")};
   if ( enumeration.isClass() ) header += QStringLiteral(" class");
   if ( !enumeration.baseName().isEmpty() ) header += QStringLiteral(" ") + enumeration.baseName();
   line->add(header);
   line->add(QStringLiteral("{"));
   *list << line;

   // Add all enumeration values parser elements to the given list, passing an
   // incremented indent by the indent spaces global setting.
   addValues(list,enumeration,indent + Settings::instance().indentSpaces());

   // Add a closing bracket line parser to the given list.
   *list << new LineParser(indent,QStringLiteral("};"));
}






/*!
 * Adds declaration parser elements for the enumeration values of the given
 * enumeration to the given declaration list using the given indent.
 *
 * @param list The declaration list that has the created parser elements of
 *             enumeration values for the given enumeration added to it.
 *
 * @param enumeration The enumeration whose enumeration value children are used
 *                    to generate parser elements.
 *
 * @param indent The indent in spaces added to all declaration parser elements
 *               created.
 */
void addValues(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent)
{
   // Make sure the given list pointer is valid.
   Q_CHECK_PTR(list);

   // Iterate through all enumeration value children blocks of the given enumeration.
   bool first {true};
   for (auto enumVal: enumeration.createListOfType<const EnumValue>(Factory::EnumValueType) )
   {
      // Add the comment block description of the enumeration value, setting its indent
      // to the one given and adding it to the given list.
      Comment* comment {new Comment};
      comment->setIndent(indent);
      comment->add(enumVal->description());
      *list << comment;

      // Create the declaration string of the enumeration value, adding a comma first if
      // this is not the first enumeration value.
      QString declaration;
      if ( first ) first = false;
      else declaration += QStringLiteral(",");
      declaration += enumVal->baseName();

      // If the enumeration value has a value then add that to its declaration string.
      if ( enumVal->hasValue() ) declaration += QStringLiteral(" = ") + enumVal->value();

      // Create a new line parser of the declaration string, setting its indent to the
      // one given and adding it to the given list.
      *list << new LineParser(indent,declaration);
   }
}






/*!
 * Determines if the given function block should be added as a definition based
 * off the given header flag, returning true if it should be added.
 *
 * @param function The function block which is evaluated.
 *
 * @param isHeader True if this is for a header file or false if it is for a
 *                 source file.
 *
 * @return True if the given function should have its definition added or false
 *         if not.
 */
bool hasDefinition(const CppQt::Function& function, bool isHeader)
{
   return ( isHeader && function.hasAnyTemplates() && !function.isPrivateMethod() )
          || ( !isHeader
               && !function.isAbstract()
               && !function.isSignal()
               && !function.isDefault()
               && !function.isDeleted()
               && function.baseName() != QStringLiteral("main")
               && ( !function.hasAnyTemplates() || function.isPrivateMethod() ) );
}






/*!
 * Adds declaration parser elements for the given function block to the given
 * declaration list adding the given indent to each parser.
 *
 * @param list The list which has the created parser elements added to it.
 *
 * @param function The function block used to generate the declaration parser
 *                 elements.
 *
 * @param indent The indent in spaces added to each parser element created.
 */
void addDeclaration(QList<Abstract::Parser*>* list, const CppQt::Function& function, int indent)
{
   // Make sure the given list pointer is valid.
   Q_CHECK_PTR(list);

   // If the given function is a signal, abstract, default, or deleted than add its
   // comments to the given declaration list.
   if ( function.isSignal()
        || function.isAbstract()
        || function.isDefault()
        || function.isDeleted() )
   {
      addComments(list,function,indent);
   }

   // Create the declaration string, initializing it with any templates arguments.
   QString declaration {createTemplate(&function)};

   // Add any function attributes that come before the function name in its
   // declaration.
   if ( function.isQtInvokable() ) declaration += QStringLiteral("Q_INVOKABLE ");
   if ( function.isExplicit() ) declaration += QStringLiteral("explicit ");
   if ( function.isVirtual() ) declaration += QStringLiteral("virtual ");
   if ( function.isConstExpr() ) declaration += QStringLiteral("constexpr ");
   if ( function.isStatic() ) declaration += QStringLiteral("static ");

   // Add the base function name and arguments to its declaration.
   declaration += createBaseDeclaration(function);

   // Add any function attributes that come after the function name to its
   // declaration.
   if ( function.isConst() ) declaration += QStringLiteral(" const");
   if ( function.isNoExcept() ) declaration += QStringLiteral(" noexcept");
   if ( function.isOverride() ) declaration += QStringLiteral(" override");
   if ( function.isFinal() ) declaration += QStringLiteral(" final");
   if ( function.isAbstract() ) declaration += QStringLiteral(" = 0");
   if ( function.isDefault() ) declaration += QStringLiteral(" = default");
   if ( function.isDeleted() ) declaration += QStringLiteral(" = delete");

   // Add the closing semicolon and then add the completed declaration line parser to
   // the given list.
   declaration += QStringLiteral(";");
   *list << new LineParser(indent,declaration);
}






/*!
 * Creates and returns the return type, base name, and arguments of the given
 * function block. Additional optional arguments can also be provided to add the
 * class scope of the function and add any initializer values of function
 * arguments.
 *
 * @param function The function block used to create and return the base name
 *                 declaration.
 *
 * @param classScope The optional class scope prepended before the base name of
 *                   the returned declaration.
 *
 * @param initializers True to add any initializer values of arguments to the
 *                     declaration or false to ignore them.
 *
 * @return Base declaration string of the given function composed of at least
 *         the base name and arguments. Can also include the class scope and
 *         argument initializer values.
 */
QString createBaseDeclaration(const CppQt::Function& function, const QString& classScope, bool initializers)
{
   // Create the return string, initializing it with the given function's return type
   // and then appending a space if it is not empty.
   QString ret {function.returnType()};
   if ( !ret.isEmpty() ) ret += QStringLiteral(" ");

   // Append the optional class scope, base name, and opening parenthesis to the
   // return string.
   ret += classScope + function.baseName() + QStringLiteral("(");

   // Iterate through all arguments of the given function block.
   bool first {true};
   for (auto argument: function.arguments())
   {
      // If this is not the first argument append a comma and space to the return
      // string.
      if ( first ) first = false;
      else ret += QStringLiteral(", ");

      // Add the argument declaration to the return string, adding any initializer if
      // the initializer flag is set to true.
      ret += argument->variableType() + QStringLiteral(" ") + argument->baseName();
      if ( initializers && argument->hasInitializer() )
      {
         ret += QStringLiteral(" = ") + argument->initializer();
      }
   }

   // Add the closing parenthesis to the now completed base declaration and return
   // it.
   ret += QStringLiteral(")");
   return ret;
}






/*!
 * Adds definition parser elements for the given function block to the given
 * definition list.
 *
 * @param list The definition list which has the created parser elements added
 *             to it.
 *
 * @param function The function block used to generate the definition parser
 *                 elements.
 */
void addDefinition(QList<Abstract::Parser*>* list, const CppQt::Function& function)
{
   // Make sure the given list pointer is valid.
   Q_CHECK_PTR(list);

   // Add the comment parser elements of the given function to the given list.
   addComments(list,function);

   // Create the class scope of the given function.
   QString classScope {createClassScope(&function)};

   // Create the header, initialized with any template arguments and the base
   // declaration of the given function.
   QString header
   {
      createTemplates(&function) + createBaseDeclaration(function,classScope,false)
   };

   // Add any flags required at the end of the header.
   if ( function.isConst() ) header += QStringLiteral(" const");
   if ( function.isNoExcept() ) header += QStringLiteral(" noexcept");

   // Create a new function parser with the completed header string and steps of
   // operation of the given function block.
   Function* functionParser
   {
      new Function(header,function.operations())
   };

   // Add the scope, base name, and opening parenthesis to the expression header of
   // the function parser.
   functionParser->add(classScope + function.baseName() + QStringLiteral("("));

   // Iterate through all arguments of the given function.
   bool first {true};
   for (auto argument: function.arguments())
   {
      // If this is not the first argument then add regular expression matching the
      // expected comma.
      if ( first ) first = false;
      else functionParser->addExp(QStringLiteral(",\\s*"));

      // Add regular expression matching the argument.
      functionParser->add(argument->variableType());
      functionParser->addExp(QStringLiteral("\\s+[a-zA-Z_]+[a-zA-Z_0-9]*\\s*"));
   }

   // Add the closing parenthesis to the expression header.
   functionParser->add(QStringLiteral(")"));

   // Add any required flags to the expression matrix followed by the closing.
   if ( function.isConst() ) functionParser->addExp(QStringLiteral("\\s+const"));
   if ( function.isNoExcept() ) functionParser->addExp(QStringLiteral("\\s+noexcept"));
   functionParser->addExp(QStringLiteral("\\s*:?\\s*\\z"));

   // Add the completed function parser to the given list.
   *list << functionParser;
}






/*!
 * Adds the comment parser element created from the given function block to the
 * given list. The created comment parser can be set to an optionally given
 * indent value.
 *
 * @param list The list which has the created comment parser added to it.
 *
 * @param function The function block used to create the comment parser.
 *
 * @param indent Optional indent in spaces added to the comment parser created.
 */
void addComments(QList<Abstract::Parser*>* list, const CppQt::Function& function, int indent)
{
   // Make sure the given list pointer is valid.
   Q_CHECK_PTR(list);

   // Create a new comment parser, setting its indent to the one possibly given and
   // adding the given function block's description.
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(function.description());

   // Add all comment descriptions of any templates the given function block contains
   // as children, separated by a single blank comment line.
   for (auto temp: function.templates())
   {
      comment->add(1);
      comment->add(QStringLiteral("@tparam ") + temp->baseName(),temp->description());
   }

   // Add all comment descriptions of any arguments the given function block contains
   // as children, separated by a single blank comment line.
   for (auto argument: function.arguments())
   {
      comment->add(1);
      comment->add(QStringLiteral("@param ") + argument->baseName(),argument->description());
   }

   // If the given function block's return value is not void and its return
   // description is not empty then add a return value comment description separated
   // by a single blank comment line.
   if ( !function.isVoidReturn() )
   {
      QString description {function.returnDescription()};
      if ( !description.isEmpty() )
      {
         comment->add(1);
         comment->add(QStringLiteral("@return"),description);
      }
   }

   // Add the completed comment parser to the given list.
   *list << comment;
}






/*!
 * Determines if the given variable block should be added as a variable
 * definition based off the given header flag, returning true if it should be
 * added.
 *
 * @param variable The variable block which is evaluated.
 *
 * @param isHeader True if this is for a header file or false if it is a source
 *                 file.
 *
 * @return True if the given variable should have its definition added or false
 *         if not.
 */
bool hasDefinition(const Variable& variable, bool isHeader)
{
   return ( isHeader
            && variable.isStatic()
            && !variable.isConstExpr()
            && variable.hasAnyTemplates() )
          || ( !isHeader
               && ( !variable.isMember()
                    || ( variable.isStatic()
                         && !variable.isConstExpr()
                         && !variable.hasAnyTemplates() ) ) );
}






/*!
 * Adds declaration parser elements for the given variable block to the given
 * declaration list with the given indent.
 *
 * @param list The declaration list which has the created declaration parser
 *             elements added to it.
 *
 * @param variable The variable block used to create the parser elements.
 *
 * @param indent The indent in spaces added to each parser element created.
 */
void addDeclaration(QList<Abstract::Parser*>* list, const Variable& variable, int indent)
{
   // Make sure the given list pointer is valid.
   Q_CHECK_PTR(list);

   // Create and add a new comment parser to the given list, setting its indent and
   // adding the given variable's description.
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(variable.description());
   *list << comment;

   // Create a declaration string, adding any variable flags that come before the
   // anything else.
   QString declaration;
   if ( variable.isConstExpr() ) declaration += QStringLiteral("constexpr ");
   if ( variable.isStatic() ) declaration += QStringLiteral("static ");
   if ( variable.isMutable() ) declaration += QStringLiteral("mutable ");
   if ( variable.isThreadLocal() ) declaration += QStringLiteral("thread_local ");
   if ( !variable.isMember() ) declaration += (QStringLiteral("extern "));

   // Add the variable type and base name to the declaration.
   declaration += variable.variableType() + QStringLiteral(" ") + variable.baseName();

   // If the given variable is a class member, is not static or is a constant
   // expression, and has an initializer then add the initializer to the declaration.
   if ( variable.isMember()
        && ( !variable.isStatic() || variable.isConstExpr() )
        && variable.hasInitializer() )
   {
      declaration += QStringLiteral(" {") + variable.initializer() + QStringLiteral("}");
   }

   // Finish the declaration with the ending semicolon and add it as a line parser to
   // the given list, setting its indent to the one given.
   declaration += QStringLiteral(";");
   *list << new LineParser(indent,declaration);
}






/*!
 * Adds definition parser elements for the given variable block to the given
 * definition list.
 *
 * @param list The definition list which has the created parser elements added
 *             to it.
 *
 * @param variable The variable block used to generate the definition parser
 *                 elements.
 */
void addDefinition(QList<Abstract::Parser*>* list, const CppQt::Variable& variable)
{
   // Create a comment parser and add the given variable's description, adding it to
   // the given list.
   Comment* comment {new Comment};
   comment->add(variable.description());
   *list << comment;

   // Create a definition string, adding thread local if that is set in the given
   // variable.
   QString definition;
   if ( variable.isThreadLocal() ) definition += QStringLiteral("thread_local ");

   // Add the type, class scope, and base name of the given variable to the
   // definition string.
   definition += variable.variableType()
                 + QStringLiteral(" ")
                 + createClassScope(&variable)
                 + variable.baseName();

   // If the given variable has an initializer then add it to the definition string.
   if ( variable.hasInitializer() )
   {
      definition += QStringLiteral(" {") + variable.initializer() + QStringLiteral("}");
   }

   // Finish the definition string with a semicolon and add it as a line parser to
   // the given list.
   definition += QStringLiteral(";");
   *list << new LineParser(0,definition);
}






/*!
 * Create and returns the class scope string for the given block. The string
 * includes the scope of any class the given block is a child of along with any
 * template arguments for each class added.
 *
 * @param block The block whose parent classes are used to generate the
 *              appropriate class scope.
 *
 * @return Class scope for the given block with any class templates included.
 */
QString createClassScope(const Abstract::Block* block)
{
   // Make sure the given block pointer is valid.
   Q_CHECK_PTR(block);

   // Create the return string.
   QString ret;

   // Move up the parents of the given block starting with the given one.
   const Abstract::Block* up {block};
   while ( up )
   {
      // If the parent block is a class then prepend its scope to the return string,
      // including any template arguments of the class and the ending double colons.
      if ( const Class* valid = up->cast<const Class>(Factory::ClassType) )
      {
         ret.prepend(valid->baseName() + createTemplate(valid,false) + QStringLiteral("::"));
      }

      // Else if the parent block is a namespace then return the finished scope string.
      else if ( up->cast<Namespace>(Factory::NamespaceType) ) return ret;

      // Move up to the next block parent.
      up = up->parent();
   }

   // Control should never be reached here but if it is return the finished scope
   // string.
   return ret;
}






/*!
 * Creates and returns the template declaration string for the given block. This
 * includes the block itself and any parent class blocks that contain templates.
 * Each block found with templates will have its separate template declaration
 * added to the return string.
 *
 * @param block The block used to create and return the template declaration.
 *
 * @return Template declaration string for the given block.
 */
QString createTemplates(const Abstract::Block* block)
{
   // Make sure the given block pointer is valid.
   Q_CHECK_PTR(block);

   // Create and return the list of template declarations for the given block.
   QString ret;
   QList<const Abstract::Block*> list {createTemplateList(block)};
   for (auto block: list) ret += createTemplate(block);
   return ret;
}






/*!
 * Creates and returns a single template declaration for the given block. An
 * optional argument is also given for formatting the returned string as a full
 * declaration for a list of templates arguments.
 *
 * @param block The block used to generate the single template declaration or
 *              argument list.
 *
 * @param declaration True to generate a full declaration or false to only
 *                    generate an argument list.
 *
 * @return Single template declaration for the given block or template arguments
 *         if optional declaration setting is disabled.
 */
QString createTemplate(const Abstract::Block* block, bool declaration)
{
   // Make sure the given block pointer is valid.
   Q_CHECK_PTR(block);

   // Create the return string.
   QString ret;

   // Get the list of templates for the given block and make sure it is not empty.
   QList<const Template*> list {block->createListOfType<const Template>(Factory::TemplateType)};
   if ( !list.isEmpty() )
   {
      // If this is a declaration add the template tag.
      if ( declaration ) ret += QStringLiteral("template");

      // Add the beginning template argument token.
      ret += QStringLiteral("<");

      // Iterate through the given block's list of templates.
      bool first {true};
      for (auto temp: list)
      {
         // Check to see if this is the first template or not.
         if ( first ) first = false;
         else
         {
            // Add a comma, followed by a space if this s a declaration.
            ret += QStringLiteral(",");
            if ( declaration ) ret += QStringLiteral(" ");
         }

         // If this is a declaration add the template type and a space.
         if ( declaration ) ret += temp->templateType() + QStringLiteral(" ");

         // Add the template's base name.
         ret += temp->baseName();
      }

      // Add the closing template argument token.
      ret += QStringLiteral("> ");
   }

   // Return the finished single template declaration or argument list.
   return ret;
}






/*!
 * Create and returns a list of all parent blocks of the given block that
 * contain templates. This list includes the given block if it contains
 * templates. The order of the returned list is from the farthest parent block
 * to the nearest.
 *
 * @param block The block used to create the list of parent blocks containing
 *              templates, including itself if it has templates.
 *
 * @return List of all parent blocks of the given block that contain templates,
 *         including the given block if it contains templates.
 */
QList<const Abstract::Block*> createTemplateList(const Abstract::Block* block)
{
   // Make sure the given block pointer is not null.
   Q_CHECK_PTR(block);

   // Create the return list of block pointers.
   QList<const Abstract::Block*> ret;

   // Move up the parents of the given block starting with the one given.
   const Abstract::Block* up {block};
   while ( up )
   {
      // If the parent block is a function or class, and it contains templates, then add
      // it to the returned list.
      if ( ( up->type() == Factory::FunctionType || up->type() == Factory::ClassType )
           && up->containsType(Factory::TemplateType) )
      {
         ret << up;
      }

      // Else if the parent block is a namespace then return the completed list.
      else if ( up->cast<Namespace>(Factory::NamespaceType) ) return ret;

      // Move up to the next parent block.
      up = up->parent();
   }

   // Control should never be reached here but if it is return the finished list of
   // template containing parent blocks.
   return ret;
}

}
}
