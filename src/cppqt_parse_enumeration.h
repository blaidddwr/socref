#ifndef CPPQT_PARSE_ENUMERATION_H
#define CPPQT_PARSE_ENUMERATION_H
#include "cppqt_parse_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the base enumeration parser. This base parser is responsible for 
       * enumeration declarations in header files. This only adds output without reading 
       * in any lines because it requires no input parsing. 
       */
      class Enumeration : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Enumeration(CppQt::Enumeration* block, AbstractParser* parent);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         /*!
          * The enumeration block that defines this parser. 
          */
         CppQt::Enumeration* _block;
         /*!
          * The number of spaces used for each indent setting. 
          */
         int _indentSpaces;
      };
   }
}



#endif
