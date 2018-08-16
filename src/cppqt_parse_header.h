#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
#include "cppqt_parse_source.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the header parser. This parses a header file for a class or namespace. 
       * Because header files can be as complicated as source files in C++ this inherits 
       * the source parser class. 
       * 
       * A header file is separated into four sections. The preprocessor, header, 
       * declaration, and definition sections in that order. The preprocessor section has 
       * the header guard and any other preprocessor statements. The header section has 
       * any custom forward class declarations. The declaration section contains the 
       * actual declarations of the class or namespace. The definition section contains 
       * any template definitions for variables and functions. The declaration and 
       * definition sections are nested inside their respective namespace scope. 
       * 
       * Lines of code inside functions are parsed and remembered just like it is in the 
       * inherited source parser class. 
       */
      class Header : public Source
      {
         Q_OBJECT
      public:
         explicit Header(const Namespace* block, const QString& name);
      protected:
         virtual void makeOutput() override final;
         virtual void readTop(const QString& line) override final;
         virtual void evaluateVariable(CppQt::Variable* block) override final;
         virtual void evaluateFunction(CppQt::Function* block) override final;
         virtual void evaluateOther(AbstractBlock* block) override final;
      private:
         void outputGuard();
         void outputDeclarations();
         void outputClassComments(const Class* block);
         void outputClassDeclaration(const Class* block);
         void outputFooter();
         /*!
          * The namespace or class block used as a root for the file this object parses. 
          */
         const Namespace* _block;
         /*!
          * The number of spaces used for each indent setting. 
          */
         int _indentSpaces;
         /*!
          * The number of blank lines before each header setting. 
          */
         int _headerLines;
         /*!
          * Pointer list of base parser objects used to generate the output of this object's 
          * declaration section. 
          */
         QList<Base*> _declarations;
         /*!
          * The first preprocessor header guard line for this object's header file. 
          */
         QString _guard1;
         /*!
          * The second preprocessor header guard line for this object's header file. 
          */
         QString _guard2;
      };
   }
}



#endif
