#ifndef CPPQT_PARSE_SOURCE_H
#define CPPQT_PARSE_SOURCE_H
#include "cppqt_parse_global.h"
#include "cppqt_parse.h"
#include "global.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the source parser. This parses a source file for a class or namespace. 
       * Because header files can be as complicated as source files in C++ this class is 
       * designed to be inherited by the header class with additional interfaces to alter 
       * its functionality. The additional interfaces mainly deal with evaluating 
       * children blocks and reading the top of the file. 
       * 
       * A source file is separated into three sections. The preprocessor, header, and 
       * definition sections in that order. The preprocessor section has all preprocessor 
       * statements most commonly being include statements. The header section has any 
       * using statements. The definition section has all variable and function 
       * definitions. 
       * 
       * The lines of code inside any function present in the source file is saved and 
       * matched to any known function of the parser object's namespace. If the function 
       * is unknown it is also saved and marked as an unknown function after the defined 
       * ones in the definition section. 
       */
      class Source : public Global
      {
         Q_OBJECT
      public:
         explicit Source(const Namespace* block, const QString& name);
      protected:
         virtual void initialize() override final;
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override;
         virtual void readTop(const QString& line);
         virtual void evaluateVariable(CppQt::Variable* block);
         virtual void evaluateFunction(CppQt::Function* block);
         virtual void evaluateOther(AbstractBlock* block);
      protected:
         explicit Source(const Namespace* block);
         void outputPreProcessor();
         void outputHeader();
         void outputDefinitions();
         Function* findDefined(const QString& definition);
         const QList<AbstractBlock*>& children() const;
         void addPreProcess(const QString& line);
         void addHeader(const QString& line);
         void addVariable(Variable* parser);
         void addDefined(Function* defined);
         void addUndefined(Function* undefined);
         bool isTemplate() const;
      private:
         void makeUsingName();
         void evaluateAll();
         /*!
          * The namespace or class block used as a root for the file this object parses. 
          */
         const Namespace* _block;
         /*!
          * The number of blank lines before each header setting. 
          */
         int _headerLines;
         /*!
          * The number of blank lines before function definitions setting. 
          */
         int _functionLines;
         /*!
          * Pointer list of all real children of this object's namespace block. 
          */
         QList<AbstractBlock*> _children;
         /*!
          * List of preprocessor lines that is added to the preprocessor section of output. 
          */
         QStringList _preProcess;
         /*!
          * List of header lines that is added to the header section of output. 
          */
         QStringList _header;
         /*!
          * Pointer list of variable parser objects that is used to create their definitions 
          * for output. 
          */
         QList<Variable*> _variables;
         /*!
          * Pointer list of defined function parser objects that is used to create their 
          * definitions for output. Defined here means that a match was found in a real 
          * child function block of this object's namespace. 
          */
         QList<Function*> _defined;
         /*!
          * Pointer list of undefined function parser objects that is used to create their 
          * definitions for output. Undefined here means that a match was not found in the 
          * real child function blocks of this object's namespace. 
          */
         QList<Function*> _undefined;
         /*!
          * True if this parser is finished reading in lines for the preprocessor and header 
          * sections of its file or false if it is past those sections. 
          */
         bool _pastTop {false};
         /*!
          * True if this object's namespace or any of its parent classes contain a template 
          * or false otherwise. By simple logic this being true also means this object's 
          * namespace is a class. 
          */
         bool _isTemplate {false};
         /*!
          * True if the lines being read in are inside a comment or false otherwise. This is 
          * used to safely ignore function signatures in comments. 
          */
         bool _inComments {false};
         /*!
          * The default using namespace declaration for this object's source file. If this 
          * source file is in global scope then this is empty. 
          */
         QString _usingName;
         /*!
          * The default include statement used to include the header file of this object's 
          * source file. 
          */
         QString _include;
      };
   }
}



#endif
