#ifndef GLSL_PARSE_H
#define GLSL_PARSE_H
#include <QMap>
#include "glsl.h"
#include "abstract.h"
#include "global.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    */
   namespace Parse
   {
      class SourceParser;
      class CommentParser;
      class FunctionParser;
      QMap<QString,Scanner*> createScannerMap(const Abstract::Block* root);
      void addSource(QMap<QString,Scanner*>* map, const Namespace& root, QString scope);
      void addSource(QMap<QString,Scanner*>* map, const Shader& shader, QString scope);
      void insert(QMap<QString,Scanner*>* map, Scanner* scanner, const Shader& shader, QString scope);
      void add(Scanner* scanner, const Variable& block, int indent);
      void add(Scanner* scanner, const Struct& block);
      QString createLayout(const Variable& block);
      void add(Scanner* scanner, const Function& block);
      void addMain(Scanner* scanner, const Shader& shader);
   }
}

#endif
