#ifndef CPPQT_PARSE_ACCESS_H
#define CPPQT_PARSE_ACCESS_H
#include "cppqt_parse_base.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Access : public Base
      {
         Q_OBJECT
      public:
         explicit Access(CppQt::Access* block, AbstractParser* parent);
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         CppQt::Access* _block;
         int _indentSpaces;
      };
   }
}



#endif
