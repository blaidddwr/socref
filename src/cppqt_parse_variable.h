#ifndef CPPQT_PARSE_VARIABLE_H
#define CPPQT_PARSE_VARIABLE_H
#include "cppqt_parse_base.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Variable : public Base
      {
         Q_OBJECT
      public:
         explicit Variable(CppQt::Variable* block, AbstractParser* parent);
         virtual void outputDetachedComments() override final;
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         CppQt::Variable* _block;
      };
   }
}



#endif
