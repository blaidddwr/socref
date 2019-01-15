#ifndef GLSL_PARSE_FUNCTION_H
#define GLSL_PARSE_FUNCTION_H
#include "glsl_parse_base.h"
#include "glsl.h"
//



namespace GLSL
{
   namespace Parse
   {
      /*!
       * This is the function parser. This is responsible for parsing all function 
       * definitions inside a shader program, including the special main function. This 
       * reads in all code lines defined within any function present in a shader source 
       * file. 
       * 
       * This parser can be defined, undefined, or the special main function. A defined 
       * function means it is linked with an actual function block. An undefined function 
       * parser means it is an unknown function definition found in the shader source 
       * file. The special main function is the main function of the shader program. 
       * 
       * Because this function parser is responsible for the special main function it 
       * must copy a reference of the operations for its function instead of simply using 
       * the function block interface. 
       */
      class Function : public Base
      {
         Q_OBJECT
      public:
         virtual void outputLines() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Function(const QString& definition, AbstractParser* parent);
         explicit Function(const GLSL::Function* block, AbstractParser* parent);
         explicit Function(const QStringList& operations, AbstractParser* parent);
         bool isMatch(const QString& line);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         void processInlineComment(const QString& line);
         void insertInlineComment(int index, int spacing);
         void makeCommentOutput();
         QString makeArguments();
         /*!
          * Pointer to this object's function block if it is defined and not the main 
          * function. If it is undefined for the main function this is null. 
          */
         const GLSL::Function* _block {nullptr};
         /*!
          * Static definition string that contains the definition header for this parser if 
          * it is undefined, otherwise this remains empty. 
          */
         QString _definition;
         /*!
          * The steps of operation for this function parser if it is defined or the special 
          * main function. If this is an undefined function parser this remains empty. 
          */
         const QStringList _operations;
         /*!
          * List of code lines for this parser object's function definition. 
          */
         QStringList _code;
         /*!
          * The current number of bracket levels this parser is in while reading in lines. 
          * Used for processing input. Each opening bracket is one level up and closing 
          * bracket is one level down. 
          */
         int _level {0};
         /*!
          * True if an inline comment is currently being read in and the first line of it 
          * has already been read or false otherwise. 
          */
         bool _edgePast {false};
         /*!
          * The next index to be used in this object's steps of operation for inserting 
          * inline comments. 
          */
         int _nextOperation {0};
      };
   }
}



#endif
