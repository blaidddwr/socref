#ifndef CPPQT_PARSE_BASE_H
#define CPPQT_PARSE_BASE_H
#include "abstractparser.h"
#include <QStringList>
#include "cppqt.h"
#include "global.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       */
      class Base : public AbstractParser
      {
         Q_OBJECT
      public:
         /*!
          */
         virtual void outputComments() = 0;
         /*!
          */
         virtual void outputDeclaration() = 0;
         /*!
          */
         virtual void outputDefinition() = 0;
      public:
         static QStringList makeComment(const QString& text, int justified = 0);
         static QStringList makeTemplateComments(const AbstractBlock* block);
         static QString getNamespace(const AbstractBlock* block);
         static QString getClassScope(const AbstractBlock* block);
         static QString getClass(const Class* block);
         static QString getTemplateDeclaration(const AbstractBlock* block);
         static QString getTemplates(const AbstractBlock* block, bool declare = false);
         explicit Base(AbstractParser* parent);
      private:
         virtual void makeOutput() override final;
      };
   }
}



#endif
