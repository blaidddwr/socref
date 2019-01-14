#ifndef GLSL_PARSE_VARIABLE_H
#define GLSL_PARSE_VARIABLE_H
#include "glsl_parse_base.h"
#include "glsl.h"
//



namespace GLSL
{
   namespace Parse
   {
      /*!
       * This is the variable parser. This outputs all variable definitions for a shader 
       * source file. This parser does not require reading in any lines of code. 
       */
      class Variable : public Base
      {
         Q_OBJECT
      public:
         virtual void makeOutput() override;
      public:
         explicit Variable(const GLSL::Variable* block, AbstractParser* parent);
      protected:
         virtual bool readLine(const QString& line) override final;
      protected:
         QString makeLayout();
      private:
         /*!
          * Pointer to the variable block that defines this parser. 
          */
         const GLSL::Variable* _block;
      };
   }
}



#endif
