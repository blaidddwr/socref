#ifndef CPPQT_PARSE_VARIABLE_H
#define CPPQT_PARSE_VARIABLE_H
#include "cppqt_parse_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the base variable parser. This base parser is responsible for parsing 
       * variables within source and header files. This only adds output without reading 
       * in any lines because variables do not require any input parsing. 
       */
      class Variable : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Variable(CppQt::Variable* block, AbstractParser* parent);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         void finishOutput(QString* line, bool withInitializer);
         /*!
          * The variable block that defines this variable parser. 
          */
         CppQt::Variable* _block;
         /*!
          * The max columns per line setting. 
          */
         int _maxColumns;
         /*!
          * The number of spaces used for each indent setting. 
          */
         int _indentSpaces;
      };
   }
}



#endif
