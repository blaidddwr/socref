#ifndef CPPQT_PARSE_DECLARATIVE_H
#define CPPQT_PARSE_DECLARATIVE_H
#include "cppqt_parse_base.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the base declarative parser. This base parser is responsible for using 
       * and friend declarations in header files. This only adds a single output with no 
       * comments or definition. This also does not read in any lines because it requires 
       * no input parsing. 
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
