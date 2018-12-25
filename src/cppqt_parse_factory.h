#ifndef CPPQT_PARSE_FACTORY_H
#define CPPQT_PARSE_FACTORY_H
#include "abstractparserfactory.h"
#include "cppqt.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the parser factory for the C++/Qt project type. This recognizes three 
       * primary file types. The naming convention for all types are the same. These 
       * types are global header files, header files, and source files. 
       * 
       * Files are named based off their fully scoped namespace name. Each namespace is 
       * separated by underscores. If the file is for common functions, variables, and 
       * enumerations outside of a class the keyword "common" is appended its name. The 
       * root namespace has the special file named global.h for its global header. 
       * 
       * The global header file has forward declarations of all classes within its 
       * namespace. The header file contains the header of either a class or a namespace. 
       * The source file contains the source of either a class or a namespace. The header 
       * and source of a namespace is simply all functions, variables, and enumerations 
       * scoped within its namespace. 
       */
      class Factory : public AbstractParserFactory
      {
         Q_OBJECT
      public:
         virtual Sut::QPtr<AbstractParser> make(const QString& name, const QString& extension) const override final;
      public:
         explicit Factory(const AbstractBlock* root);
      private:
         AbstractParser* find(const Namespace* current, const QStringList& names, const QString& name, bool isHeader, int index = 0) const;
         AbstractParser* findCommon(const Namespace* current, const QStringList& names, const QString& name, bool isHeader, int index = 0) const;
         const CppQt::Namespace* findNamespace(const Namespace* current, const QString& name) const;
         /*!
          * Pointer to the root block used by this parser factory and all parser objects it 
          * makes. 
          */
         const Namespace* _root;
      };
   }
}



#endif
