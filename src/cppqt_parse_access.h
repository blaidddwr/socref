#ifndef CPPQT_PARSE_ACCESS_H
#define CPPQT_PARSE_ACCESS_H
#include "cppqt_parse_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the base access parser. This base parser is responsible for access 
       * declarations such as public or private in class declarations in header files. 
       * This only adds output without reading in any lines because it requires no input 
       * parsing. 
       */
      class Access : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Access(CppQt::Access* block, AbstractParser* parent);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         /*!
          * The access block that defines this access parser. 
          */
         CppQt::Access* _block;
         /*!
          * The number of spaces used for each indent setting. 
          */
         int _indentSpaces;
      };
   }
}



#endif
