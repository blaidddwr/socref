#ifndef CPPQT_PARSE_H
#define CPPQT_PARSE_H
#include <QMap>
#include "cppqt.h"
#include "abstract.h"
#include "basic.h"
#include "global.h"



/*!
 * Contains the C++/Qt project type.
 */
namespace CppQt
{
   /*!
    * Contains all custom parser elements and parsing functions for the C++/Qt
    * project type.
    */
   namespace Parse
   {
      class Header;
      class Source;
      class Comment;
      class Function;
      QMap<QString,Scanner*> createScannerMap(const Abstract::Block* root);
      void addMain(QMap<QString,Scanner*>* map, const Namespace& root);
      void addHeader(QMap<QString,Scanner*>* map, const Namespace& root);
      void addHeader(QMap<QString,Scanner*>* map, const Class& root);
      void addSource(QMap<QString,Scanner*>* map, const Namespace& root);
      void addSource(QMap<QString,Scanner*>* map, const Class& root);
      void addParsers(Scanner* scanner, QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, QList<const Namespace*>* scope = nullptr);
      void addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name = nullptr);
      void addSourceHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name = nullptr);
      void addFooter(Scanner* scanner);
      void insertScanner(QMap<QString,Scanner*>* map, Scanner* scanner, const QList<const Namespace*>& scope, const QString& ending, const Class* name = nullptr);
      QList<const Namespace*> createScope(const Abstract::Block& block);
      void addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope, bool flat = false, bool describe = false);
      void endScope(Scanner* scanner, int* indent, bool flat = false);
      void createParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Abstract::Block& block, int indent, bool isHeader);
      void createClassParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Class& root, int indent, bool isHeader);
      void add(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent);
      void addValues(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent);
      bool hasDefinition(const CppQt::Function& function, bool isHeader);
      void addDeclaration(QList<Abstract::Parser*>* list, const CppQt::Function& function, int indent);
      QString createBaseDeclaration(const CppQt::Function& function, const QString& classScope = QString(), bool initializers = true);
      void addDefinition(QList<Abstract::Parser*>* list, const CppQt::Function& function);
      void addComments(QList<Abstract::Parser*>* list, const CppQt::Function& function, int indent = 0);
      bool hasDefinition(const Variable& variable, bool isHeader);
      void addDeclaration(QList<Abstract::Parser*>* list, const Variable& variable, int indent);
      void addDefinition(QList<Abstract::Parser*>* list, const CppQt::Variable& variable);
      QString createClassScope(const Abstract::Block* block);
      QString createTemplates(const Abstract::Block* block);
      QString createTemplate(const Abstract::Block* block);
      QList<const Abstract::Block*> createTemplateList(const Abstract::Block* block);
   }
}

#endif
