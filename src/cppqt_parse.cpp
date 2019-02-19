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
 *
 *
 *
 *
 *
 *
 */
void createParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Abstract::Block& block, int indent, bool isHeader)
{
   using LineParser = Basic::LineParser;
   const int functionLines {Settings::instance().functionLines()};
   for (auto child: block.list())
   {
      if ( const Enumeration* valid = child->cast<const Enumeration>(Factory::EnumerationType) )
      {
         if ( declarations ) add(declarations,*valid,indent);
      }
      else if ( const Class* valid = child->cast<const Class>(Factory::ClassType) )
      {
         if ( block.type() == Factory::AccessType )
         {
            createClassParsers(declarations,variables,functions,*valid,indent,isHeader);
         }
         else if ( declarations && !valid->hasAnyTemplates() )
         {
            *declarations << new LineParser(indent
                                            ,QStringLiteral("class ")
                                             + valid->baseName()
                                             + QStringLiteral(";"));
         }
      }
      else if ( const Variable* valid = child->cast<const Variable>(Factory::VariableType) )
      {
         if ( declarations ) addDeclaration(declarations,*valid,indent);
         if ( variables && hasDefinition(*valid,isHeader) ) addDefinition(variables,*valid);
      }
      else if ( const CppQt::Function* valid = child->cast<const CppQt::Function>(Factory::FunctionType) )
      {
         if ( declarations )
         {
            addDeclaration(declarations,*valid,indent);
         }
         if ( functions && hasDefinition(*valid,isHeader) )
         {
            if ( !functions->isEmpty() ) *functions << new LineParser(functionLines);
            addDefinition(functions,*valid);
         }
      }
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
 *
 *
 *
 *
 *
 *
 */
void createClassParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Class& root, int indent, bool isHeader)
{
   using LineParser = Basic::LineParser;
   const int indentSpaces {Settings::instance().indentSpaces()};

   if ( declarations )
   {
      Comment* comment {new Comment};
      comment->setIndent(indent);
      comment->add(root.description());
      for (auto temp: root.templates())
      {
         comment->add(1);
         comment->add(QStringLiteral("@tparam ") + temp->baseName(),temp->description());
      }
      *declarations << comment;
      QString header {QStringLiteral("class ") + root.baseName()};
      QList<const Parent*> parents {root.createListOfType<const Parent>(Factory::ParentType)};
      if ( !parents.isEmpty() )
      {
         header += QStringLiteral(" : ");
         bool first {true};
         for (auto name: parents)
         {
            if ( first ) first = false;
            else header += QStringLiteral(", ");
            header += name->accessString() + QStringLiteral(" ") + name->className();
         }
      }
      LineParser* line {new LineParser(indent,header)};
      line->add(QStringLiteral("{"));
      *declarations << line;
      if ( root.isQtObject() )
      {
         *declarations << new LineParser(indent + indentSpaces,QStringLiteral("Q_OBJECT"));
      }
   }

   for (auto access: root.createListOfType<const Access>(Factory::AccessType))
   {
      if ( declarations ) *declarations << new LineParser(indent,access->accessString());
      createParsers(declarations,variables,functions,*access,indent + indentSpaces,isHeader);
   }

   if ( declarations ) *declarations << new LineParser(indent,QStringLiteral("};"));
}






/*!
 *
 *
 *
 */
void add(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent)
{
   Q_CHECK_PTR(list);
   using LineParser = Basic::LineParser;
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(enumeration.description());
   *list << comment;
   LineParser* line {new LineParser};
   line->setIndent(indent);
   QString header {QStringLiteral("enum")};
   if ( enumeration.isClass() ) header += QStringLiteral(" class");
   if ( !enumeration.baseName().isEmpty() ) header += QStringLiteral(" ") + enumeration.baseName();
   line->add(header);
   line->add(QStringLiteral("{"));
   *list << line;
   addValues(list,enumeration,indent + Settings::instance().indentSpaces());
   *list << new LineParser(indent,QStringLiteral("};"));
}






/*!
 *
 *
 *
 */
void addValues(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent)
{
   Q_CHECK_PTR(list);
   using LineParser = Basic::LineParser;
   bool first {true};
   for (auto child: enumeration.list())
   {
      if ( const EnumValue* enumVal = child->cast<EnumValue>(Factory::EnumValueType) )
      {
         Comment* comment {new Comment};
         comment->setIndent(indent);
         comment->add(enumVal->description());
         *list << comment;
         QString definition;
         if ( first ) first = false;
         else definition += QStringLiteral(",");
         definition += enumVal->baseName();
         if ( enumVal->hasValue() )
         {
            definition += QStringLiteral(" = ") + enumVal->value();
         }
         *list << new LineParser(indent,definition);
      }
   }
}






/*!
 *
 *
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
 *
 *
 *
 */
void addDeclaration(QList<Abstract::Parser*>* list, const CppQt::Function& function, int indent)
{
   Q_CHECK_PTR(list);
   using LineParser = Basic::LineParser;
   if ( function.isSignal()
        || function.isAbstract()
        || function.isDefault()
        || function.isDeleted() )
   {
      addComments(list,function,indent);
   }
   QString declaration {createTemplate(&function)};
   if ( function.isQtInvokable() ) declaration += QStringLiteral("Q_INVOKABLE ");
   if ( function.isExplicit() ) declaration += QStringLiteral("explicit ");
   if ( function.isVirtual() ) declaration += QStringLiteral("virtual ");
   if ( function.isConstExpr() ) declaration += QStringLiteral("constexpr ");
   if ( function.isStatic() ) declaration += QStringLiteral("static ");
   declaration += createBaseDeclaration(function);
   if ( function.isConst() ) declaration += QStringLiteral(" const");
   if ( function.isNoExcept() ) declaration += QStringLiteral(" noexcept");
   if ( function.isOverride() ) declaration += QStringLiteral(" override");
   if ( function.isFinal() ) declaration += QStringLiteral(" final");
   if ( function.isAbstract() ) declaration += QStringLiteral(" = 0");
   if ( function.isDefault() ) declaration += QStringLiteral(" = default");
   if ( function.isDeleted() ) declaration += QStringLiteral(" = delete");
   declaration += QStringLiteral(";");
   *list << new LineParser(indent,declaration);
}






/*!
 *
 *
 *
 */
QString createBaseDeclaration(const CppQt::Function& function, const QString& classScope, bool initializers)
{
   QString ret {function.returnType()};
   if ( !ret.isEmpty() ) ret += QStringLiteral(" ");
   ret += classScope + function.baseName() + QStringLiteral("(");
   bool first {true};
   for (auto argument: function.arguments())
   {
      if ( first ) first = false;
      else ret += QStringLiteral(", ");
      ret += argument->variableType() + QStringLiteral(" ") + argument->baseName();
      if ( initializers && argument->hasInitializer() )
      {
         ret += QStringLiteral(" = ") + argument->initializer();
      }
   }
   ret += QStringLiteral(")");
   return ret;
}






/*!
 *
 *
 */
void addDefinition(QList<Abstract::Parser*>* list, const CppQt::Function& function)
{
   Q_CHECK_PTR(list);
   addComments(list,function);
   QString classScope {createClassScope(&function)};
   QString header
   {
      createTemplates(&function) + createBaseDeclaration(function,classScope,false)
   };
   if ( function.isConst() ) header += QStringLiteral(" const");
   if ( function.isNoExcept() ) header += QStringLiteral(" noexcept");
   Function* functionParser
   {
      new Function(header,function.operations())
   };
   functionParser->add(classScope + function.baseName() + QStringLiteral("("));
   bool first {true};
   for (auto argument: function.arguments())
   {
      if ( first ) first = false;
      else functionParser->addExp(QStringLiteral(",\\s*"));
      functionParser->add(argument->variableType());
      functionParser->addExp(QStringLiteral("\\s+[a-zA-Z_]+[a-zA-Z_0-9]*\\s*"));
   }
   functionParser->add(QStringLiteral(")"));
   if ( function.isConst() ) functionParser->addExp(QStringLiteral("\\s+const"));
   if ( function.isNoExcept() ) functionParser->addExp(QStringLiteral("\\s+noexcept"));
   functionParser->addExp(QStringLiteral("\\s*:?\\s*\\z"));
   *list << functionParser;
}






/*!
 *
 *
 *
 */
void addComments(QList<Abstract::Parser*>* list, const CppQt::Function& function, int indent)
{
   Q_CHECK_PTR(list);
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(function.description());
   for (auto temp: function.templates())
   {
      comment->add(1);
      comment->add(QStringLiteral("@tparam ") + temp->baseName(),temp->description());
   }
   for (auto argument: function.arguments())
   {
      comment->add(1);
      comment->add(QStringLiteral("@param ") + argument->baseName(),argument->description());
   }
   if ( !function.isVoidReturn() )
   {
      QString description {function.returnDescription()};
      if ( !description.isEmpty() )
      {
         comment->add(1);
         comment->add(QStringLiteral("@return"),description);
      }
   }
   *list << comment;
}






/*!
 *
 *
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
 *
 *
 *
 */
void addDeclaration(QList<Abstract::Parser*>* list, const Variable& variable, int indent)
{
   Q_CHECK_PTR(list);
   using LineParser = Basic::LineParser;
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(variable.description());
   *list << comment;
   QString declaration;
   if ( variable.isConstExpr() ) declaration += QStringLiteral("constexpr ");
   if ( variable.isStatic() ) declaration += QStringLiteral("static ");
   if ( variable.isMutable() ) declaration += QStringLiteral("mutable ");
   if ( variable.isThreadLocal() ) declaration += QStringLiteral("thread_local ");
   if ( !variable.isMember() ) declaration += (QStringLiteral("extern "));
   declaration += variable.variableType() + QStringLiteral(" ") + variable.baseName();
   if ( variable.isMember()
        && ( !variable.isStatic() || variable.isConstExpr() )
        && variable.hasInitializer() )
   {
      declaration += QStringLiteral(" {") + variable.initializer() + QStringLiteral("}");
   }
   declaration += QStringLiteral(";");
   *list << new LineParser(indent,declaration);
}






/*!
 *
 *
 */
void addDefinition(QList<Abstract::Parser*>* list, const CppQt::Variable& variable)
{
   using LineParser = Basic::LineParser;
   Comment* comment {new Comment};
   comment->add(variable.description());
   *list << comment;
   QString definition;
   if ( variable.isThreadLocal() ) definition += QStringLiteral("thread_local ");
   definition += variable.variableType()
                 + QStringLiteral(" ")
                 + createClassScope(&variable)
                 + variable.baseName();
   if ( variable.hasInitializer() )
   {
      definition += QStringLiteral(" {") + variable.initializer() + QStringLiteral("}");
   }
   definition += QStringLiteral(";");
   *list << new LineParser(0,definition);
}






/*!
 *
 */
QString createClassScope(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QString ret;
   const Abstract::Block* up {block};
   while ( up )
   {
      if ( const Class* valid = up->cast<const Class>(Factory::ClassType) )
      {
         ret.prepend(valid->baseName() + QStringLiteral("::"));
      }
      else if ( up->cast<Namespace>(Factory::NamespaceType) ) return ret;
      up = up->parent();
   }
   return ret;
}






/*!
 *
 */
QString createTemplates(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QString ret;
   QList<const Abstract::Block*> list {createTemplateList(block)};
   for (auto block: list) ret += createTemplate(block);
   return ret;
}






/*!
 *
 */
QString createTemplate(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QString ret;
   QList<const Template*> list {block->createListOfType<const Template>(Factory::TemplateType)};
   if ( !list.isEmpty() )
   {
      ret += QStringLiteral("template<");
      bool first {true};
      for (auto temp: list)
      {
         if ( first ) first = false;
         else ret += QStringLiteral(", ");
         ret += temp->templateType() + QStringLiteral(" ") + temp->baseName();
      }
      ret += QStringLiteral("> ");
   }
   return ret;
}






/*!
 *
 */
QList<const Abstract::Block*> createTemplateList(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QList<const Abstract::Block*> ret;
   const Abstract::Block* up {block};
   while ( up )
   {
      if ( up->type() == Factory::FunctionType || up->type() == Factory::ClassType ) ret << up;
      else if ( up->cast<Namespace>(Factory::NamespaceType) ) return ret;
      up = up->parent();
   }
   return ret;
}

}
}
