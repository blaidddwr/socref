#ifndef CPPQT_PARSE_ENUMERATION_H
#define CPPQT_PARSE_ENUMERATION_H
#include "cppqt_parse_base.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Enumeration : public Base
      {
         Q_OBJECT
      public:
         explicit Enumeration(CppQt::Enumeration* block, AbstractParser* parent);
         virtual void outputDetachedComments() override final;
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         CppQt::Enumeration* _block;
      };
   }
}



#endif
