#include "cppqt_parse_common.h"
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
#include "cppqt_access.h"
#include "cppqt_function.h"
#include "basic_lineparser.h"
#include "scanner.h"



using namespace CppQt;
//






/*!
 *
 * @param variable  
 *
 * @param isHeader  
 */
bool Parse::hasDefinition(const Variable& variable, bool isHeader)
{
   return ( isHeader && variable.isStatic() && variable.hasAnyTemplates() )
          || ( !isHeader && ( !variable.isMember() || ( variable.isStatic() && !variable.hasAnyTemplates() ) ) );
}






/*!
 *
 * @param function  
 *
 * @param isHeader  
 */
bool Parse::hasDefinition(const CppQt::Function& function, bool isHeader)
{
   return ( isHeader && function.hasAnyTemplates() && !function.isPrivateMethod() )
          || ( !isHeader && ( !function.hasAnyTemplates() || function.isPrivateMethod() ) );
}






/*!
 *
 * @param declarations  
 *
 * @param variables  
 *
 * @param functions  
 *
 * @param block  
 *
 * @param indent  
 *
 * @param isHeader  
 */
void Parse::createParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Abstract::Block& block, int indent, bool isHeader)
{
   using LineParser = Basic::LineParser;
   const int functionLines {Settings::instance().functionLines()};
   for (auto child: block.list())
   {
      if ( const Enumeration* valid = child->cast<const Enumeration>(Factory::EnumerationType) )
      {
         if ( declarations ) addEnumeration(declarations,*valid,indent);
      }
      else if ( const Class* valid = child->cast<const Class>(Factory::ClassType) )
      {
         if ( block.type() == Factory::ClassType ) createClassParsers(declarations,variables,functions,*valid,indent,isHeader);
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
         if ( declarations ) addVariableDeclaration(declarations,*valid,indent);
         if ( variables && hasDefinition(*valid,isHeader) )
         {
            addVariableDefinition(variables,*valid);
         }
      }
      else if ( const CppQt::Function* valid = child->cast<const CppQt::Function>(Factory::FunctionType) )
      {
         if ( declarations )
         {
            *declarations << new LineParser(indent
                                            ,createFunctionDeclaration(*valid)
                                             + QStringLiteral(";"));
         }
         if ( functions && hasDefinition(*valid,isHeader) )
         {
            if ( !functions->isEmpty() ) *functions << new LineParser(functionLines);
            addFunctionDefinition(functions,*valid);
         }
      }
   }
}






/*!
 *
 * @param scanner  
 *
 * @param declarations  
 *
 * @param variables  
 *
 * @param functions  
 *
 * @param scope  
 */
void Parse::addParsers(Scanner* scanner, QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, QList<const Namespace*>* scope)
{
   Q_CHECK_PTR(scanner);
   const int headerLines {Settings::instance().headerLines()};
   const int functionLines {Settings::instance().functionLines()};

   using LineParser = Basic::LineParser;
   int indent {0};
   if ( declarations && !declarations->isEmpty() )
   {
      scanner->addParser(new LineParser(headerLines));
      if ( scope ) addScope(scanner,&indent,*scope,false,true);
      for (auto parser: *declarations) scanner->addParser(parser);
      if ( scope ) endScope(scanner,&indent);
   }

   if ( variables && !variables->isEmpty() )
   {
      scanner->addParser(new LineParser(headerLines));
      if ( scope ) addScope(scanner,&indent,*scope);
      for (auto parser: *variables)
      {
         parser->setIndent(indent);
         scanner->addParser(parser);
      }
      if ( scope ) endScope(scanner,&indent);
   }

   if ( functions && !functions->isEmpty() )
   {
      scanner->addParser(new LineParser(functionLines));
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
 *
 * @param root  
 */
QMap<QString,Scanner*> Parse::createScannerMap(const Abstract::Block* root)
{
   Q_CHECK_PTR(root);
   QMap<QString,Scanner*> ret;
   const Namespace* namespaceRoot {root->cast<const Namespace>(Factory::NamespaceType)};
   Q_CHECK_PTR(namespaceRoot);
   addHeader(&ret,*namespaceRoot);
   addSource(&ret,*namespaceRoot);
   return ret;
}






/*!
 *
 * @param map  
 *
 * @param root  
 */
void Parse::addHeader(QMap<QString,Scanner*>* map, const Namespace& root)
{
   Q_CHECK_PTR(map);

   const int indentSpaces {Settings::instance().indentSpaces()};

   Scanner* scanner {new Scanner};

   // .
   QList<const Namespace*> scope {createScope(root)};
   addHeader(scanner,scope);

   QList<Abstract::Parser*> declarations;
   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createParsers(&declarations,&variables,&functions,root,scope.size()*indentSpaces,true);
   addParsers(scanner,&declarations,&variables,&functions,&scope);

   // .
   addFooter(scanner);

   // .
   insertScanner(map,scanner,scope,QStringLiteral(".h"));

   //. recursive iterate child namespaces
   QList<const Namespace*> spaces {root.createListOfType<const Namespace>(Factory::NamespaceType)};
   for(auto space: spaces) addHeader(map,*space);

   //. iterate child classes
   QList<const Class*> objects {root.createListOfType<const Class>(Factory::ClassType)};
   for(auto object: objects) addHeader(map,*object);
}






/*!
 *
 * @param map  
 *
 * @param root  
 */
void Parse::addHeader(QMap<QString,Scanner*>* map, const Class& root)
{
   Q_CHECK_PTR(map);

   const int indentSpaces {Settings::instance().indentSpaces()};

   Scanner* scanner {new Scanner};

   // .
   QList<const Namespace*> scope {createScope(root)};
   addHeader(scanner,scope,&root);

   QList<Abstract::Parser*> declarations;
   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createClassParsers(&declarations,&variables,&functions,root,scope.size()*indentSpaces,true);
   addParsers(scanner,&declarations,&variables,&functions,&scope);

   // .
   addFooter(scanner);

   // .
   insertScanner(map,scanner,scope,QStringLiteral(".h"),&root);
}






/*!
 *
 * @param map  
 *
 * @param root  
 */
void Parse::addSource(QMap<QString,Scanner*>* map, const Namespace& root)
{
   Q_CHECK_PTR(map);

   const int headerLines {Settings::instance().headerLines()};

   using LineParser = Basic::LineParser;
   Scanner* scanner {new Scanner};

   // .
   QList<const Namespace*> scope {createScope(root)};
   addSourceHeader(scanner,scope);

   int indent {0};
   if ( !scope.isEmpty() ) scanner->addParser(new LineParser(headerLines));
   addScope(scanner,&indent,scope,true);

   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createParsers(nullptr,&variables,&functions,root,0,false);
   addParsers(scanner,nullptr,&variables,&functions);

   scanner->addParser(new LineParser(1));
   endScope(scanner,&indent,true);
   scanner->addParser(new LineParser(1));

   insertScanner(map,scanner,scope,QStringLiteral(".cpp"));

   //. recursive iterate child namespaces
   QList<const Namespace*> spaces {root.createListOfType<const Namespace>(Factory::NamespaceType)};
   for(auto space: spaces) addSource(map,*space);
}






/*!
 *
 * @param scanner  
 *
 * @param scope  
 *
 * @param name  
 */
void Parse::addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name)
{
   Q_CHECK_PTR(scanner);
   using LineParser = Basic::LineParser;
   QString headerScope;
   bool first {true};
   for (auto name: scope)
   {
      if ( first ) first = false;
      else headerScope += QStringLiteral("_");
      headerScope += name->baseName().toUpper();
   }
   if ( name )
   {
      if ( !first ) headerScope += QStringLiteral("_");
      headerScope += name->baseName().toUpper();
   }
   if ( headerScope.isEmpty() ) headerScope += QStringLiteral("GLOBAL");
   LineParser* line {new LineParser};
   line->add(QStringLiteral("#ifndef ") + headerScope + QStringLiteral("_H"));
   line->add(QStringLiteral("#define ") + headerScope + QStringLiteral("_H"));
   scanner->addParser(line);
   scanner->addParser(new Header);
}






/*!
 *
 * @param scanner  
 *
 * @param scope  
 */
void Parse::addSourceHeader(Scanner* scanner, const QList<const Namespace*>& scope)
{
   Q_CHECK_PTR(scanner);
   using LineParser = Basic::LineParser;
   QString headerScope;
   bool first {true};
   for (auto name: scope)
   {
      if ( first ) first = false;
      else headerScope += QStringLiteral("_");
      headerScope += name->baseName().toLower();
   }
   if ( headerScope.isEmpty() ) headerScope += QStringLiteral("global");
   LineParser* line
   {
      new LineParser(0,QStringLiteral("#include \"") + headerScope + QStringLiteral(".h\""))
   };
   scanner->addParser(line);
   scanner->addParser(new Source);
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
 * @param map  
 *
 * @param scanner  
 *
 * @param scope  
 *
 * @param ending  
 *
 * @param name  
 */
void Parse::insertScanner(QMap<QString,Scanner*>* map, Scanner* scanner, const QList<const Namespace*>& scope, const QString& ending, const Class* name)
{
   QString fileName;
   bool first {true};
   for (auto name: scope)
   {
      if ( first ) first = false;
      else fileName += QStringLiteral("_");
      fileName += name->baseName().toLower();
   }
   if ( name )
   {
      if ( !first ) fileName += QStringLiteral("_");
      fileName += name->baseName().toLower();
   }
   if ( fileName.isEmpty() ) fileName += QStringLiteral("global");
   fileName += ending;
   Q_ASSERT(!map->contains(fileName));
   map->insert(fileName,scanner);
}






/*!
 *
 * @param block  
 */
QList<const Namespace*> Parse::createScope(const Abstract::Block& block)
{
   QList<const Namespace*> ret;
   const Abstract::Block* up {&block};
   while ( up->parent() )
   {
      if ( const Namespace* valid = up->cast<Namespace>(Factory::NamespaceType) )
      {
         ret.prepend(valid);
      }
      up = up->parent();
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
 *
 * @param flat  
 *
 * @param describe  
 */
void Parse::addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope, bool flat, bool describe)
{
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);
   using LineParser = Basic::LineParser;
   for (auto name: scope)
   {
      if ( describe )
      {
         Comment* comment {new Comment};
         comment->setIndent(*indent);
         comment->add(name->description());
         scanner->addParser(comment);
      }
      LineParser* line
      {
         new LineParser(flat? 0 : *indent,QStringLiteral("namespace ") + name->baseName())
      };
      line->add(QStringLiteral("{"));
      scanner->addParser(line);
      *indent += Settings::instance().indentSpaces();
   }
}






/*!
 *
 * @param scanner  
 *
 * @param indent  
 *
 * @param flat  
 */
void Parse::endScope(Scanner* scanner, int* indent, bool flat)
{
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);
   using LineParser = Basic::LineParser;
   while ( *indent > 0 )
   {
      *indent -= Settings::instance().indentSpaces();
      scanner->addParser(new LineParser(flat? 0 : *indent,QStringLiteral("}")));
   }
}






/*!
 *
 * @param declarations  
 *
 * @param variables  
 *
 * @param functions  
 *
 * @param root  
 *
 * @param indent  
 *
 * @param isHeader  
 */
void Parse::createClassParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Class& root, int indent, bool isHeader)
{
   using LineParser = Basic::LineParser;
   const int indentSpaces {Settings::instance().indentSpaces()};

   if ( declarations )
   {
      LineParser* line {new LineParser(indent,QStringLiteral("class ") + root.baseName())};
      line->add(QStringLiteral("{"));
      *declarations << line;
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
 * @param list  
 *
 * @param enumeration  
 *
 * @param indent  
 */
void Parse::addEnumeration(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent)
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
   addEnumValues(list,enumeration,indent + Settings::instance().indentSpaces());
   *list << new LineParser(indent,QStringLiteral("};"));
}






/*!
 *
 * @param list  
 *
 * @param enumeration  
 *
 * @param indent  
 */
void Parse::addEnumValues(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent)
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
 * @param list  
 *
 * @param variable  
 *
 * @param indent  
 */
void Parse::addVariableDeclaration(QList<Abstract::Parser*>* list, const Variable& variable, int indent)
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
   if ( variable.isMember() && !variable.isStatic() && variable.hasInitializer() )
   {
      declaration += QStringLiteral(" {") + variable.initializer() + QStringLiteral("}");
   }
   declaration += QStringLiteral(";");
   *list << new LineParser(indent,declaration);
}






/*!
 *
 * @param function  
 */
QString Parse::createFunctionDeclaration(const CppQt::Function& function)
{
   QString ret {createTemplate(&function)};
   if ( function.isQtInvokable() ) ret += QStringLiteral("Q_INVOKABLE ");
   if ( function.isExplicit() ) ret += QStringLiteral("explicit ");
   if ( function.isVirtual() ) ret += QStringLiteral("virtual ");
   if ( function.isConstExpr() ) ret += QStringLiteral("constexpr ");
   if ( function.isStatic() ) ret += QStringLiteral("static ");
   ret += createBaseFunctionDeclaration(function);
   if ( function.isConst() ) ret += QStringLiteral(" const");
   if ( function.isNoExcept() ) ret += QStringLiteral(" noexcept");
   if ( function.isOverride() ) ret += QStringLiteral(" override");
   if ( function.isFinal() ) ret += QStringLiteral(" final");
   if ( function.isAbstract() ) ret += QStringLiteral(" = 0");
   if ( function.isDefault() ) ret += QStringLiteral(" = default");
   if ( function.isDeleted() ) ret += QStringLiteral(" = delete");
   return ret;
}






/*!
 *
 * @param function  
 *
 * @param classScope  
 */
QString Parse::createBaseFunctionDeclaration(const CppQt::Function& function, const QString& classScope)
{
   QString ret
   {
      function.returnType()
      + QStringLiteral(" ")
      + classScope
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
 * @param list  
 *
 * @param function  
 */
void Parse::addFunctionDefinition(QList<Abstract::Parser*>* list, const CppQt::Function& function)
{
   Q_CHECK_PTR(list);
   addFunctionComments(list,function);
   QString classScope {createClassScope(&function)};
   QString header
   {
      createTemplates(&function) + createBaseFunctionDeclaration(function,classScope)
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
      functionParser->addExp(argument->variableType());
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
 * @param list  
 *
 * @param variable  
 */
void Parse::addVariableDefinition(QList<Abstract::Parser*>* list, const CppQt::Variable& variable)
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
 * @param list  
 *
 * @param function  
 */
void Parse::addFunctionComments(QList<Abstract::Parser*>* list, const CppQt::Function& function)
{
   Q_CHECK_PTR(list);
   Comment* comment {new Comment};
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
 * @param block  
 */
QString Parse::createClassScope(const Abstract::Block* block)
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
 * @param block  
 */
QString Parse::createTemplates(const Abstract::Block* block)
{
   Q_CHECK_PTR(block);
   QString ret;
   QList<const Abstract::Block*> list {createTemplateList(block)};
   for (auto block: list) ret += createTemplate(block);
   return ret;
}






/*!
 *
 * @param block  
 */
QString Parse::createTemplate(const Abstract::Block* block)
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
 * @param block  
 */
QList<const Abstract::Block*> Parse::createTemplateList(const Abstract::Block* block)
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
