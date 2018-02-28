#ifndef CPPQT_PARSE_COMMON_H
#define CPPQT_PARSE_COMMON_H
#include <QStringList>
#include "cppqt.h"
#include "global.h"



namespace CppQt
{
   namespace Parse
   {
      QStringList makeTemplateComments(const AbstractBlock* block);
      QStringList makeComment(const QString& text, int justified = 0);
      QString getNamespace(const AbstractBlock* block);
      QString getClassScope(const AbstractBlock* block);
      QString getClass(const Class* block);
      QString getTemplateDeclaration(const AbstractBlock* block);
      QString getTemplates(const AbstractBlock* block, bool declare = false);
   }
}



#endif
