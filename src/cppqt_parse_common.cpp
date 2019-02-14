#include "cppqt_parse_common.h"
#include "cppqt_parse_header.h"
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

   const int headerLines {Settings::instance().headerLines()};
   const int functionLines {Settings::instance().functionLines()};
   const int indentSpaces {Settings::instance().indentSpaces()};

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
      scanner->addParser(new LineParser(headerLines));
      addScope(scanner,&indent,indentSpaces,scope);
      for (auto enumBlock: enumerations) addEnumeration(scanner,*enumBlock,indent);
      endScope(scanner,&indent,indentSpaces);
   }

   // .
   QList<const Class*> classes {root->createListOfType<const Class>(Factory::ClassType)};
   if ( !classes.isEmpty() )
   {
      scanner->addParser(new LineParser(headerLines));
      addScope(scanner,&indent,indentSpaces,scope);
      addForwardClasses(scanner,classes,indent);
      endScope(scanner,&indent,indentSpaces);
   }

   //. function headers
   QList<const CppQt::Function*> functions
   {
      root->createListOfType<const CppQt::Function>(Factory::FunctionType)
   };
   if ( !functions.isEmpty() )
   {
      scanner->addParser(new LineParser(headerLines));
      addScope(scanner,&indent,indentSpaces,scope);
      LineParser* line {new LineParser};
      line->setIndent(indent);
      for (auto function: functions)
      {
         line->add(createFunctionDeclaration(*function) + QStringLiteral(";"));
      }
      scanner->addParser(line);
      endScope(scanner,&indent,indentSpaces);
   }

   //. variables
   QList<const Variable*> variables
   {
      root->createListOfType<const Variable>(Factory::VariableType)
   };
   if ( !variables.isEmpty() )
   {
      scanner->addParser(new LineParser(headerLines));
      addScope(scanner,&indent,indentSpaces,scope);
      LineParser* line {new LineParser};
      line->setIndent(indent);
      for (auto variable: variables)
      {
         line->add(QStringLiteral("extern ")
                   + createVariableDeclaration(*variable)
                   + QStringLiteral(";"));
      }
      scanner->addParser(line);
      endScope(scanner,&indent,indentSpaces);
   }

   //. function definitions
   if ( !functions.isEmpty() )
   {
      for (auto function: functions)
      {
         if ( function->hasAnyTemplates() && !function->isPrivateMethod() )
         {
            scanner->addParser(new LineParser(functionLines));
            addScope(scanner,&indent,indentSpaces,scope);
            addFunctionDefinition(scanner,*function,indent);
            endScope(scanner,&indent,indentSpaces);
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
   QList<const Namespace*> spaces {root->createListOfType<const Namespace>(Factory::NamespaceType)};
   for(auto space: spaces) addHeader(map,space);

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
 * @param change  
 *
 * @param scope  
 */
void Parse::addScope(Scanner* scanner, int* indent, int change, const QList<const Namespace*>& scope)
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
      *indent += change;
   }
}






/*!
 *
 * @param scanner  
 *
 * @param indent  
 *
 * @param change  
 */
void Parse::endScope(Scanner* scanner, int* indent, int change)
{
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);
   using LineParser = Basic::LineParser;
   while ( *indent > 0 )
   {
      *indent -= change;
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
   addEnumValues(scanner,enumeration,indent + Settings::instance().indentSpaces());
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
   for (auto object: classes)
   {
      if ( !object->hasTemplates() )
      {
         line->add(QStringLiteral("class ") + object->baseName() + QStringLiteral(";"));
      }
   }
   scanner->addParser(line);
}






/*!
 *
 * @param function  
 */
QString Parse::createFunctionDeclaration(const CppQt::Function& function)
{
   QString ret
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
      else ret += QStringLiteral(", ");
      ret += argument->variableType() + QStringLiteral(" ") + argument->baseName();
   }
   ret += QStringLiteral(")");
   return ret;
}






/*!
 *
 * @param variable  
 */
QString Parse::createVariableDeclaration(const CppQt::Variable& variable)
{
   return variable.variableType() + QStringLiteral(" ") + variable.baseName();
}






/*!
 *
 * @param scanner  
 *
 * @param function  
 *
 * @param indent  
 */
void Parse::addFunctionDefinition(Scanner* scanner, const CppQt::Function& function, int indent)
{
   Q_CHECK_PTR(scanner);
   addFunctionComments(scanner,function,indent);
   Function* functionParser
   {
      new Function(createFunctionDeclaration(function),function.operations())
   };
   functionParser->setIndent(indent);
   functionParser->add(function.baseName() + QStringLiteral("("));
   bool first {true};
   for (auto argument: function.arguments())
   {
      functionParser->addExp(QStringLiteral("\\s*"));
      if ( first ) first = false;
      else functionParser->addExp(QStringLiteral(","));
      functionParser->addExp(argument->variableType());
      functionParser->addExp(QStringLiteral("\\s+[a-zA-Z_]+[a-zA-Z_0-9]*\\s*"));
   }
   functionParser->add(QStringLiteral(")"));
   if ( function.isConst() ) functionParser->addExp(QStringLiteral("\\s+const"));
   if ( function.isNoExcept() ) functionParser->addExp(QStringLiteral("\\s+noexcept"));
   functionParser->addExp(QStringLiteral("\\s*:?\\s*\\z"));
   scanner->addParser(functionParser);
}






/*!
 *
 * @param scanner  
 *
 * @param function  
 *
 * @param indent  
 */
void Parse::addFunctionComments(Scanner* scanner, const CppQt::Function& function, int indent)
{
   Q_CHECK_PTR(scanner);
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
   scanner->addParser(comment);
}






/*!
 *
 * @param block  
 */
QString Parse::createTemplates(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QString ret;
   QList<QList<const Template*>> superList {createTemplateList(block)};
   for (auto list: superList)
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
 * @param block  
 */
QList<QList<const Template*>> Parse::createTemplateList(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QList<QList<const Template*>> ret;
   const Abstract::Block* up {block};
   while ( up )
   {
      if ( up->type() == Factory::FunctionType || up->type() == Factory::ClassType )
      {
         QList<const Template*> list
         {
            up->createListOfType<const Template>(Factory::TemplateType)
         };
         if ( !list.isEmpty() ) ret.prepend(list);
      }
      else if ( up->cast<Namespace>(Factory::NamespaceType) ) return ret;
      up = up->parent();
   }
   return ret;
}
