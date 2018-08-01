#ifndef CPPQT_PARSE_FORWARD_H
#define CPPQT_PARSE_FORWARD_H
#include "cppqt_parse_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the base forward parser. This base parser is responsible for forward 
       * class declarations in class declarations in header files. This only adds output 
       * without reading in any lines because it requires no input parsing. 
       */
      class Forward : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Forward(Class* block, AbstractParser* parent);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         /*!
          * The class block that defines this forward parser. 
          */
         Class* _block;
      };
   }
}



#endif
