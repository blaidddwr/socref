#ifndef CPPQT_PARSE_FUNCTION_H
#define CPPQT_PARSE_FUNCTION_H
#include "cppqt_parse_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the base function parser. This base parser is responsible for parsing 
       * functions within source and header files. This also reads in the lines of code 
       * for any function found in source or header files. Function definitions can exist 
       * in either source or header files in C++ because of templates. 
       * 
       * This parser can be defined or undefined. A defined function parser means it is 
       * linked with an actual function block. An undefined function parser means it is 
       * an unknown function definition found in a source or header file with no 
       * associated function block. 
       * 
       * This parser also has the concept of a cut off. The cut off is an integer that 
       * defines the number of additional spaces the function itself is indented in its 
       * file that can be ignored and effectively cut off when parsing the lines of code. 
       */
      class Function : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         explicit Function(CppQt::Function* block, AbstractParser* parent);
         explicit Function(const QString& definition, AbstractParser* parent);
         bool isMatch(const QString& line);
         bool hasCode() const;
         void setCutOff(int value);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         void processInlineComment(const QString& line);
         void insertInlineComment(int index, int spacing);
         void outputArgumentComments();
         void outputReturnDescriptionComment();
         QString makeReturnValue();
         QString makeName(bool isRegExp = false);
         QString makeArguments(bool withInitializers);
         /*!
          * Pointer to this object's function block if it is defined or null if it is 
          * undefined. 
          */
         CppQt::Function* _block {nullptr};
         /*!
          * The max columns per line setting. 
          */
         int _maxColumns;
         /*!
          * Static definition string that contains the definition header for this parser if 
          * it is undefined or an empty string if it is defined. 
          */
         QString _definition;
         /*!
          * The current number of bracket levels this parser is in while reading in lines. 
          * Used for processing input. Each opening bracket is one level up and closing 
          * bracket is one level down. 
          */
         int _level {0};
         /*!
          * The number of additional spaces the definition of this object's function has in 
          * its file. This is used for processing input. 
          */
         int _cutOff {0};
         /*!
          * True if an inline comment is currently being read in and the first line of it 
          * has already been read or false otherwise. 
          */
         bool _edgePast {false};
         /*!
          * The next index to be used in this object's function's steps of operation for 
          * inserting inline comments. 
          */
         int _nextOperation {0};
         /*!
          * List of code lines for this object's function's definition. 
          */
         QStringList _code;
         /*!
          * List of initializer lines for this object's function's definition. This is 
          * exclusively used for constructors. 
          */
         QStringList _initializers;
      };
   }
}



#endif
