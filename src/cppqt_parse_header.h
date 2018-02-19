#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
#include "cppqt_parse_source.h"



namespace CppQt
{
   namespace Parse
   {
      class Header : public Source
      {
         Q_OBJECT
      public:
         Header(Namespace* block, const QString& name);
      protected:
         virtual void makeOutput() override final;
         virtual void readTop(const QString& line) override final;
         virtual void evaluateVariable(CppQt::Variable* block) override final;
         virtual void evaluateFunction(CppQt::Function* block) override final;
         virtual void evaluateOther(AbstractBlock* block) override final;
      private:
         void outputHeader();
         void outputDeclarations();
         void outputClassComments(Class* block);
         void outputClassDeclaration(Class* block);
         void outputFooter();
         Namespace* _block;
         QList<Base*> _declarations;
         QString _header1;
         QString _header2;
      };
   }
}



#endif
