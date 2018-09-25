#ifndef CPPQT_PARSE_DECLARATIVE_H
#define CPPQT_PARSE_DECLARATIVE_H
#include "cppqt_parse_base.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       */
      class Declarative : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Declarative(CppQt::Using* block, AbstractParser* parent);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         /*!
          * The using or friend block that defines this parser. 
          */
         CppQt::Using* _block;
      };
   }
}



#endif
