#ifndef GLSL_PARSE_STRUCT_H
#define GLSL_PARSE_STRUCT_H
#include "glsl_parse_variable.h"
//



namespace GLSL
{
   namespace Parse
   {
      /*!
       * This is the struct parser. This outputs all struct definitions, including the 
       * variable definitions contained within the struct, for a shader source file. This 
       * parser does not require reading in any lines of code. 
       */
      class Struct : public Variable
      {
         Q_OBJECT
      public:
         explicit Struct(const GLSL::Struct* block, AbstractParser* parent);
      protected:
         virtual void makeOutput() override final;
      private:
         void makeVariables();
         /*!
          * Pointer to the struct block that defines this parser. 
          */
         const GLSL::Struct* _block;
      };
   }
}



#endif
