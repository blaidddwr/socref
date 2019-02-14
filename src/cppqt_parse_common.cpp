#include "cppqt_parse_common.h"
#include "cppqt_parse_header.h"
#include "cppqt_parse_comment.h"
#include "cppqt_parse_function.h"
#include "cppqt_factory.h"
#include "cppqt_namespace.h"
#include "cppqt_enumeration.h"
#include "cppqt_enumvalue.h"
#include "cppqt_variable.h"
#include "cppqt_template.h"
#include "cppqt_class.h"
#include "cppqt_function.h"
#include "basic_lineparser.h"
#include "scanner.h"



using namespace CppQt;
//






/*!
 *
 * @param root  
 */
QMap<QString,Scanner*> Parse::createScannerMap(const Abstract::Block* root)
{
   Q_CHECK_PTR(root);
   QMap<QString,Scanner*> ret;
   const Namespace* namespaceRoot {root->cast<const Namespace>(Factory::NamespaceType)};
   Q_CHECK_PTR(namespaceRoot);
   addHeader(&ret,namespaceRoot);
   return ret;
}






/*!
 *
 * @param map  
 *
 * @param root  
 */
void Parse::addHeader(QMap<QString,Scanner*>* map, const Namespace* root)
{
   Q_CHECK_PTR(root);
   Q_CHECK_PTR(map);

   QString baseName;
   if ( root->parent() ) baseName = root->baseName();
   else baseName = QStringLiteral("global");

   using LineParser = Basic::LineParser;
   Scanner* scanner {new Scanner};

   // .
   QList<const Namespace*> scope {createScope(root)};
   addHeader(scanner,scope,baseName);

   // .
   int indent {0};

   // .
   QList<const Enumeration*> enumerations
   {
      root->createListOfType<const Enumeration>(Factory::EnumerationType)
   };
   if ( !enumerations.isEmpty() )
   {
      scanner->addParser(new LineParser(3));
      addScope(scanner,&indent,scope);
      for (auto enumBlock: enumerations) addEnumeration(scanner,*enumBlock,indent);
      endScope(scanner,&indent);
   }

   // .
   QList<const Class*> classes {root->createListOfType<const Class>(Factory::ClassType)};
   if ( !classes.isEmpty() )
   {
      scanner->addParser(new LineParser(3));
      addScope(scanner,&indent,scope);
      addForwardClasses(scanner,classes,indent);
      endScope(scanner,&indent);
   }

   //. function headers
   QList<const CppQt::Function*> functions
   {
      root->createListOfType<const CppQt::Function>(Factory::FunctionType)
   };
   if ( !functions.isEmpty() )
   {
      scanner->addParser(new LineParser(3));
      addScope(scanner,&indent,scope);
      LineParser* line {new LineParser};
      line->setIndent(indent);
      for (auto function: functions) addFunctionDeclaration(line,*function);
      scanner->addParser(line);
      endScope(scanner,&indent);
   }

   //. function definitions
   if ( !functions.isEmpty() )
   {
      for (auto func: functions)
      {
         if ( func->hasAnyTemplates() && !func->isPrivateMethod() )
         {
            scanner->addParser(new LineParser(6));
            addScope(scanner,&indent,scope);
            addFunctionDefinition(scanner,*func);
            endScope(scanner,&indent);
         }
      }
   }

   // .
   addFooter(scanner);

   // .
   QString fileName;
   for (auto name: scope) fileName += name->baseName().toLower() + QStringLiteral("_");
   fileName += baseName.toLower() + QStringLiteral(".h");
   Q_ASSERT(!map->contains(fileName));
   map->insert(fileName,scanner);

   //. recursive iterate child namespaces

   //. iterate child classes
}






/*!
 *
 * @param scanner  
 *
 * @param scope  
 *
 * @param baseName  
 */
void Parse::addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const QString& baseName)
{
   Q_CHECK_PTR(scanner);
   using LineParser = Basic::LineParser;
   QString headerScope;
   for (auto name: scope) headerScope += name->baseName().toUpper() + QStringLiteral("_");
   LineParser* line {new LineParser};
   line->add(QStringLiteral("#ifndef ") + headerScope + baseName.toUpper() + QStringLiteral("_H"));
   line->add(QStringLiteral("#define ") + headerScope + baseName.toUpper() + QStringLiteral("_H"));
   scanner->addParser(line);
   scanner->addParser(new Header);
}






/*!
 *
 * @param scanner  
 */
void Parse::addFooter(Scanner* scanner)
{
   using LineParser = Basic::LineParser;
   LineParser* line {new LineParser};
   line->add(1);
   line->add(QStringLiteral("#endif"));
   line->add(1);
   scanner->addParser(line);
}






/*!
 *
 * @param block  
 */
QList<const Namespace*> Parse::createScope(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QList<const Namespace*> ret;
   const Abstract::Block* parent {block->parent()};
   while ( parent )
   {
      if ( const Namespace* valid = parent->cast<Namespace>(Factory::NamespaceType) )
      {
         ret.prepend(valid);
      }
      parent = parent->parent();
   }
   return ret;
}






/*!
 *
 * @param scanner  
 *
 * @param indent  
 *
 * @param scope  
 */
void Parse::addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope)
{
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);
   using LineParser = Basic::LineParser;
   for (auto name: scope)
   {
      LineParser* line
      {
         new LineParser(*indent,QStringLiteral("namespace ") + name->baseName())
      };
      line->add(QStringLiteral("{"));
      scanner->addParser(line);
      *indent += 3;
   }
}






/*!
 *
 * @param scanner  
 *
 * @param indent  
 */
void Parse::endScope(Scanner* scanner, int* indent)
{
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);
   using LineParser = Basic::LineParser;
   while ( *indent > 0 )
   {
      *indent -= 3;
      scanner->addParser(new LineParser(*indent,QStringLiteral("}")));
   }
}






/*!
 *
 * @param scanner  
 *
 * @param enumeration  
 *
 * @param indent  
 */
void Parse::addEnumeration(Scanner* scanner, const Enumeration& enumeration, int indent)
{
   Q_CHECK_PTR(scanner);
   using LineParser = Basic::LineParser;
   Comment* comment {new Comment};
   comment->setIndent(indent);
   comment->add(enumeration.description());
   scanner->addParser(comment);
   LineParser* line {new LineParser};
   line->setIndent(indent);
   QString header {QStringLiteral("enum")};
   if ( enumeration.isClass() ) header += QStringLiteral(" class");
   if ( !enumeration.baseName().isEmpty() ) header += QStringLiteral(" ") + enumeration.baseName();
   line->add(header);
   line->add(QStringLiteral("{"));
   scanner->addParser(line);
   addEnumValues(scanner,enumeration,indent + 3);
   line = new LineParser;
   line->setIndent(indent);
   line->add(QStringLiteral("}"));
   scanner->addParser(line);
}






/*!
 *
 * @param scanner  
 *
 * @param enumeration  
 *
 * @param indent  
 */
void Parse::addEnumValues(Scanner* scanner, const Enumeration& enumeration, int indent)
{
   Q_CHECK_PTR(scanner);
   using LineParser = Basic::LineParser;
   bool first {true};
   for (auto child: enumeration.list())
   {
      if ( const EnumValue* enumVal = child->cast<EnumValue>(Factory::EnumValueType) )
      {
         Comment* comment {new Comment};
         comment->setIndent(indent);
         comment->add(enumVal->description());
         scanner->addParser(comment);
         QString definition;
         if ( first ) first = false;
         else definition += QStringLiteral(",");
         definition += enumVal->baseName();
         if ( enumVal->hasValue() )
         {
            definition += QStringLiteral(" = ") + enumVal->value();
         }
         scanner->addParser(new LineParser(indent,definition));
      }
   }
}






/*!
 *
 * @param scanner  
 *
 * @param classes  
 *
 * @param indent  
 */
void Parse::addForwardClasses(Scanner* scanner, QList<const Class*>& classes, int indent)
{
   Q_CHECK_PTR(scanner);
   using LineParser = Basic::LineParser;
   LineParser* line = {new LineParser};
   line->setIndent(indent);
   for (auto name: classes)
   {
      line->add(QStringLiteral("class ") + name->baseName() + QStringLiteral(";"));
   }
   scanner->addParser(line);
}






/*!
 *
 * @param parser  
 *
 * @param function  
 */
void Parse::addFunctionDeclaration(Basic::LineParser* parser, const CppQt::Function& function)
{
   Q_CHECK_PTR(parser);
   QString declaration
   {
      createTemplates(&function)
      + function.returnType()
      + QStringLiteral(" ")
      + function.baseName()
      + QStringLiteral("(")
   };
   bool first {true};
   for (auto argument: function.arguments())
   {
      if ( first ) first = false;
      else declaration += QStringLiteral(", ");
      declaration += argument->variableType() + QStringLiteral(" ") + argument->baseName();
   }
   declaration += QStringLiteral(");");
   parser->add(declaration);
}






/*!
 *
 * @param scanner  
 *
 * @param func  
 */
void Parse::addFunctionDefinition(Scanner* scanner, const CppQt::Function& func)
{
   Q_CHECK_PTR(scanner);
   Function* function {new Function(func.operations())};
   function->add(func.baseName() + QStringLiteral("("));
   bool first {true};
   for (auto argument: func.arguments())
   {
      function->addExp(QStringLiteral("\\s*"));
      if ( first ) first = false;
      else function->addExp(QStringLiteral(","));
      function->addExp(argument->variableType());
      function->addExp(QStringLiteral("\\s+[a-zA-Z_]+[a-zA-Z_0-9]*\\s*"));
   }
   function->add(QStringLiteral(")"));
   if ( func.isConst() ) function->addExp(QStringLiteral("\\s+const"));
   if ( func.isNoExcept() ) function->addExp(QStringLiteral("\\s+noexcept"));
   function->addExp(QStringLiteral("\\s*:?\\s*\\z"));
   scanner->addParser(function);
}






/*!
 *
 * @param block  
 */
QString Parse::createTemplates(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QString ret;
   const Abstract::Block* up {block};
   while ( up )
   {
      if ( const CppQt::Function* valid = up->cast<CppQt::Function>(Factory::FunctionType) )
      {
         ret.prepend(createTemplate(valid));
      }
      else if ( const Class* valid = up->cast<Class>(Factory::ClassType) )
      {
         ret.prepend(createTemplate(valid));
      }
      else if ( up->cast<Namespace>(Factory::NamespaceType) ) return ret;
      up = up->parent();
   }
   return ret;
}






/*!
 *
 * @param block  
 */
QString Parse::createTemplate(const Abstract::Block* block)
{
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
