#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
#include "cppqt_parse_base.h"
#include "cppqt_parse.h"



namespace CppQt
{
   namespace Parse
   {
      class Header : public Base
      {
         Q_OBJECT
      public:
         Header(Namespace* block);
      protected:
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
      private:
         void outputPreProcesser();
         void outputHeader();
         void outputClassDefinition(Class* block);
         void outputDeclarations(AbstractBlock* block);
         void outputFunctions();
         void outputVariable(const Variable* block);
         void outputFunction(const CppQt::Function* block);
         void outputEnumeration(const Enumeration* block);
         Function* findDefined(const QString& definition);
         void buildDefined();
         Namespace* _block;
         QStringList _preprocesser;
         QStringList _header;
         QList<Function*> _defined;
         QList<Function*> _undefined;
         bool _pastHeader {false};
      };
   }
}



#endif
