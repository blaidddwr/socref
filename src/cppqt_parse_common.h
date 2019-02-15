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
      QMap<QString,Scanner*> createScannerMap(const Abstract::Block* root);
      void addHeader(QMap<QString,Scanner*>* map, const Namespace* root);
      void addSource(QMap<QString,Scanner*>* map, const Namespace* root);
      void addHeader(Scanner* scanner, const QList<const Namespace*>& scope);
      void addSourceHeader(Scanner* scanner, const QList<const Namespace*>& scope);
      void addFooter(Scanner* scanner);
      void insertScanner(QMap<QString,Scanner*>* map, Scanner* scanner, const QList<const Namespace*>& scope, const QString& ending);
      QList<const Namespace*> createScope(const Abstract::Block* block);
      void addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope, bool flat = false);
      void endScope(Scanner* scanner, int* indent, bool flat = false);
      void addEnumeration(Scanner* scanner, const Enumeration& enumeration, int indent);
      void addEnumValues(Scanner* scanner, const Enumeration& enumeration, int indent);
      void addForwardClasses(Scanner* scanner, QList<const Class*>& classes, int indent);
      QString createFunctionDeclaration(const CppQt::Function& function);
      QString createVariableDeclaration(const CppQt::Variable& variable);
      void addFunctionDefinition(Scanner* scanner, const CppQt::Function& function, int indent);
      void addVariableDefinition(Scanner* scanner, const CppQt::Variable& variable);
      void addFunctionComments(Scanner* scanner, const CppQt::Function& function, int indent);
      QString createTemplates(const Abstract::Block* block);
      QList<QList<const Template*>> createTemplateList(const Abstract::Block* block);
   }
}



#endif
