#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
#include "cppqt_parse_global.h"
#include "cppqt_parse.h"
#include "global.h"



namespace CppQt
{
   namespace Parse
   {
      class Header : public Global
      {
         Q_OBJECT
      public:
         Header(Namespace* block, const QString& name);
      protected:
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
      private:
         void outputHeader();
         void outputPreProcesser();
         void outputMisc();
         void outputDeclarations();
         void outputClassComments(Class* block);
         void outputClassDeclaration(Class* block);
         void outputDefinitions();
         void outputFooter();
         Function* findDefined(const QString& definition);
         void buildDeclarations();
         Namespace* _block;
         QStringList _preprocesser;
         QStringList _misc;
         QList<Base*> _declarations;
         QList<Variable*> _variables;
         QList<Function*> _defined;
         QList<Function*> _undefined;
         bool _pastHeader {false};
         bool _isTemplate;
         QString _header1;
         QString _header2;
      };
   }
}



#endif
