#ifndef GLSL_PARSE_SHADER_H
#define GLSL_PARSE_SHADER_H
#include "glsl_parse_base.h"
#include "glsl_parse.h"
#include "glsl.h"
//



namespace GLSL
{
   namespace Parse
   {
      /*!
       * This is the shader root parser. This parser is responsible for parsing a shader 
       * program file. It collects all defined variables, structures, and functions and 
       * outputs them to the source file. It also reads in any undefined functions found 
       * in the source file and outputs them again as an unknown function. 
       */
      class Shader : public Base
      {
         Q_OBJECT
      public:
         explicit Shader(const GLSL::Shader* block);
      protected:
         virtual void initialize() override final;
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
      private:
         void evaluate();
         Function* findDefined(const QString& definition);
         /*!
          * Pointer to the shader block that defines this parser. 
          */
         const GLSL::Shader* _block;
         /*!
          * List of parser children that is part of this shader object's shader program. 
          */
         QList<Base*> _children;
         /*!
          * List of defined function parser children that is part of this shader object's 
          * shader program. 
          */
         QList<Function*> _defined;
         /*!
          * The special main function parser that is part of this shader object's shader 
          * program. 
          */
         Function* _main;
         /*!
          * List preprocessor lines for the shader program source file of this parser. 
          */
         QStringList _preProcess;
      };
   }
}



#endif
