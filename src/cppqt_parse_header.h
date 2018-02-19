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
         Header(Namespace* block);
      protected:
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
      private:
         void outputPreProcesser();
         void outputHeader();
         void outputDeclarations();
         void outputClassDeclaration(Class* block);
         void outputDefinitions();
         Function* findDefined(const QString& definition);
         void buildDeclarations();
         Namespace* _block;
         QStringList _preprocesser;
         QStringList _header;
         QList<Base*> _declarations;
         QList<Function*> _defined;
         QList<Function*> _undefined;
         bool _pastHeader {false};
      };
   }
}



#endif
