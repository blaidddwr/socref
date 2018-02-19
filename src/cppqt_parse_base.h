#ifndef CPPQT_PARSE_BASE_H
#define CPPQT_PARSE_BASE_H
#include "abstractparser.h"
#include "global.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Base : public AbstractParser
      {
         Q_OBJECT
      public:
         explicit Base() = default;
         explicit Base(AbstractParser* parent);
      protected:
         void beginNamespaceNesting(AbstractBlock* block);
         void endNamespaceNesting();
         QString variableDefinition(const Variable* block, bool isHeader);
         QString functionDefinition(const Function* block, bool withTemplate, bool withScope, bool withInitializers, bool allProperties);
         void outputComment(const QString& text, int justified = 0);
         void appendClass(const AbstractBlock* block, QString* line, bool hasTemplate = false);
      private:
         void appendClassTemplate(const Class* block, QString* line, bool declare = false);
         void appendNamespace(const AbstractBlock* block, QString* line);
         void appendArguments(const Function* block, QString* line, bool withInitializers);
         int _nestDepth {0};
      };
   }
}



#endif
