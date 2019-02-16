#ifndef CPPQT_PARSE_COMMON_H
#define CPPQT_PARSE_COMMON_H
#include <QMap>
#include "cppqt.h"
#include "abstract.h"
#include "basic.h"
#include "global.h"
//



namespace CppQt
{
   namespace Parse
   {
      bool hasDefinition(const Variable& variable, bool isHeader);
      bool hasDefinition(const CppQt::Function& function, bool isHeader);
      void createParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Abstract::Block& block, int indent, bool isHeader);
      void addParsers(Scanner* scanner, QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, QList<const Namespace*>* scope = nullptr);
      QMap<QString,Scanner*> createScannerMap(const Abstract::Block* root);
      void addHeader(QMap<QString,Scanner*>* map, const Namespace& root);
      void addHeader(QMap<QString,Scanner*>* map, const Class& root);
      void addSource(QMap<QString,Scanner*>* map, const Namespace& root);
      void addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const Class* name = nullptr);
      void addSourceHeader(Scanner* scanner, const QList<const Namespace*>& scope);
      void addFooter(Scanner* scanner);
      void insertScanner(QMap<QString,Scanner*>* map, Scanner* scanner, const QList<const Namespace*>& scope, const QString& ending, const Class* name = nullptr);
      QList<const Namespace*> createScope(const Abstract::Block& block);
      void addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope, bool flat = false, bool describe = false);
      void endScope(Scanner* scanner, int* indent, bool flat = false);
      void createClassParsers(QList<Abstract::Parser*>* declarations, QList<Abstract::Parser*>* variables, QList<Abstract::Parser*>* functions, const Class& root, int indent, bool isHeader);
      void addEnumeration(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent);
      void addEnumValues(QList<Abstract::Parser*>* list, const Enumeration& enumeration, int indent);
      void addVariableDeclaration(QList<Abstract::Parser*>* list, const Variable& variable, int indent);
      QString createFunctionDeclaration(const CppQt::Function& function);
      QString createBaseFunctionDeclaration(const CppQt::Function& function, const QString& classScope = QString());
      void addFunctionDefinition(QList<Abstract::Parser*>* list, const CppQt::Function& function);
      void addVariableDefinition(QList<Abstract::Parser*>* list, const CppQt::Variable& variable);
      void addFunctionComments(QList<Abstract::Parser*>* list, const CppQt::Function& function);
      QString createClassScope(const Abstract::Block* block);
      QString createTemplates(const Abstract::Block* block);
      QString createTemplate(const Abstract::Block* block);
      QList<const Abstract::Block*> createTemplateList(const Abstract::Block* block);
   }
}



#endif
