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
      void addHeader(Scanner* scanner, const QList<const Namespace*>& scope, const QString& baseName);
      void addFooter(Scanner* scanner);
      QList<const Namespace*> createScope(const Abstract::Block* block);
      void addScope(Scanner* scanner, int* indent, const QList<const Namespace*>& scope);
      void endScope(Scanner* scanner, int* indent);
      void addEnumeration(Scanner* scanner, const Enumeration& enumeration, int indent);
      void addEnumValues(Scanner* scanner, const Enumeration& enumeration, int indent);
      void addForwardClasses(Scanner* scanner, QList<const Class*>& classes, int indent);
      void addFunctionDeclaration(Basic::LineParser* parser, const CppQt::Function& function);
      void addFunctionDefinition(Scanner* scanner, const CppQt::Function& func);
      QString createTemplates(const Abstract::Block* block);
      QString createTemplate(const Abstract::Block* block);
   }
}



#endif
