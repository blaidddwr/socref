#ifndef CPPQT_PARSE_COMMONHEADER_H
#define CPPQT_PARSE_COMMONHEADER_H
#include "abstractparser.h"
#include "cppqt.h"
#include "cppqt_parse.h"



namespace CppQt
{
   namespace Parse
   {
      class CommonHeader : public AbstractParser
      {
      public:
         CommonHeader(Namespace* common);
      protected:
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
      private:
         void outputPreProcesser();
         void outputHeader();
         void outputDeclarations();
         void outputVariable(CppQt::Variable* block);
         void outputEnumeration(CppQt::Enumeration* block);
         void outputFunction(CppQt::Function* block);
         void outputFunctions();
         Function* findDefined(const QString& header);
         void buildDefined();
         Namespace* _common;
         QStringList _preprocesser;
         QStringList _header;
         QList<Function*> _defined;
         QList<Function*> _undefined;
      };
   }
}



#endif
