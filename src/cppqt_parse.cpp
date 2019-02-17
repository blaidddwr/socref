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



namespace CppQt
{
namespace Parse
{






/*!
 *
 */
QMap<QString,Scanner*> createScannerMap(const Abstract::Block* root)
{
   Q_CHECK_PTR(root);
   QMap<QString,Scanner*> ret;
   const Namespace* namespaceRoot {root->cast<const Namespace>(Factory::NamespaceType)};
   Q_CHECK_PTR(namespaceRoot);
   addMain(&ret,*namespaceRoot);
   addHeader(&ret,*namespaceRoot);
   addSource(&ret,*namespaceRoot);
   return ret;
}






/*!
 *
 *
 */
void addMain(QMap<QString,Scanner*>* map, const Namespace& root)
{
   Q_CHECK_PTR(map);
   if ( map->contains(QStringLiteral("main.cpp")) ) return;
   const int functionLines {Settings::instance().functionLines()};
   using LineParser = Basic::LineParser;
   for (auto function: root.createListOfType<const CppQt::Function>(Factory::FunctionType))
   {
      if ( function->baseName() == QStringLiteral("main") )
      {
         Scanner* scanner {new Scanner};
         scanner->addParser(new Source(true));
         scanner->addParser(new LineParser(functionLines));
         QList<Abstract::Parser*> lines;
         addDefinition(&lines,*function);
         for (auto line: lines) scanner->addParser(line);
         scanner->addParser(new LineParser(1));
         map->insert(QStringLiteral("main.cpp"),scanner);
         return;
      }
   }
}






/*!
 *
 *
 */
void addHeader(QMap<QString,Scanner*>* map, const Namespace& root)
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
 *
 */
void addHeader(QMap<QString,Scanner*>* map, const Class& root)
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
 *
 */
void addSource(QMap<QString,Scanner*>* map, const Namespace& root)
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

   //. recursive iterate child classes
   QList<const Class*> objects {root.createListOfType<const Class>(Factory::ClassType)};
   for(auto object: objects) addSource(map,*object);
}






/*!
 *
 *
 */
void addSource(QMap<QString,Scanner*>* map, const Class& root)
{
   Q_CHECK_PTR(map);

   const int headerLines {Settings::instance().headerLines()};

   using LineParser = Basic::LineParser;
   Scanner* scanner {new Scanner};

   // .
   QList<const Namespace*> scope {createScope(root)};
   addSourceHeader(scanner,scope,&root);

   int indent {0};
   if ( !scope.isEmpty() ) scanner->addParser(new LineParser(headerLines));
   addScope(scanner,&indent,scope,true);

   QList<Abstract::Parser*> variables;
   QList<Abstract::Parser*> functions;
   createClassParsers(nullptr,&variables,&functions,root,0,false);
   addParsers(scanner,nullptr,&variables,&functions);

   scanner->addParser(new LineParser(1));
   endScope(scanner,&indent,true);
   scanner->addParser(new LineParser(1));

   insertScanner(map,scanner,scope,QStringLiteral(".cpp"),&root);
}






/*!
 *
 *
 *
 *
 *
 */
void addParsers(Scanner* scanner, QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, QList<const Namespace*>* scope)
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
 *
 *
 */
void addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name)
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
 *
 *
 */
void addSourceHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name)
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
   if ( name )
   {
      if ( !first ) headerScope += QStringLiteral("_");
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
 */
void addFooter(Scanner* scanner)
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
 *
 *
 *
 *
 */
void insertScanner(QMap<QString,Scanner*>* map, Scanner* scanner, const QList<const Namespace*>& scope, const QString& ending, const Class* name)
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
   if ( map->contains(fileName) )
   {
      qDebug().noquote() << QObject::tr("WARNING: scanner with file name key '%1' already exists, discarding duplicate scanner!")
                            .arg(fileName);
      delete scanner;
   }
   else map->insert(fileName,scanner);
}






/*!
 *
 */
QList<const Namespace*> createScope(const Abstract::Block& block)
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
 *
 *
 *
 *
 */
void addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope, bool flat, bool describe)
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
      line->add(QStringLiteral("{"));//}
      scanner->addParser(line);
      *indent += Settings::instance().indentSpaces();
   }
}






/*!
 *
 *
 *
 */
void endScope(Scanner* scanner, int* indent, bool flat)
{
   Q_CHECK_PTR(scanner);
   Q_CHECK_PTR(indent);
   using LineParser = Basic::LineParser;
   while ( *indent > 0 )
   {
      *indent -= Settings::instance().indentSpaces();
      scanner->addParser(new LineParser(flat? 0 : *indent,QStringLiteral("}")));//{
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
         if ( block.type() == Factory::AccessType ) createClassParsers(declarations,variables,functions,*valid,indent,isHeader);
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
