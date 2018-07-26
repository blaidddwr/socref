#ifndef CPPQT_PARSE_GLOBAL_H
#define CPPQT_PARSE_GLOBAL_H
#include "abstractparser.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the global header parser. This parser is responsible for global header 
       * files which provide a list of forward declarations for all classes in the 
       * namespace the header file represents. This parser does not actually parse its 
       * file instead simply overwriting it with the appropriate forward declarations. 
       * This is inherited by the source and header parser classes for its namespace 
       * nesting output functionality. 
       */
      class Global : public AbstractParser
      {
         Q_OBJECT
      public:
         explicit Global(const Namespace* block);
      protected:
         virtual bool readLine(const QString& line) override;
         virtual void makeOutput() override;
      protected:
         void beginNamespaceNesting(bool outputLast = true);
         void endNamespaceNesting();
      private:
         void outputNamespace(const Namespace* block);
         /*!
          * The namespace block represented by the file of this parser. 
          */
         const Namespace* _block;
         /*!
          * The global indent spaces setting. 
          */
         int _indentSpaces;
         /*!
          * The number of namespace declarations outputted without closing brackets by this 
          * parser. 
          */
         int _depth {0};
      };
   }
}



#endif
