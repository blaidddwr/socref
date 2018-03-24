#ifndef CPPQT_PARSE_DECLARATION_H
#define CPPQT_PARSE_DECLARATION_H
#include "cppqt_parse_base.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Declaration : public Base
      {
         Q_OBJECT
      public:
         explicit Declaration(CppQt::Declaration* block, AbstractParser* parent);
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         CppQt::Declaration* _block;
         int _indentSpaces;
      };
   }
}



#endif
