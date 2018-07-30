#ifndef CPPQT_PARSE_BASE_H
#define CPPQT_PARSE_BASE_H
#include "abstractparser.h"
#include <QStringList>
#include "cppqt.h"
#include "global.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This base class represents parser objects that can belong to root parser objects 
       * for C++/Qt projects. These base objects include things such as functions or 
       * variables that are part of a root parser such as a header or source file. The 
       * root parser then uses its base parser children to deal with parsing of its 
       * individual components such as functions or variables. 
       * 
       * This class prevents the make output interface to be used, instead defining its 
       * more specialized output interfaces that are specific to C++ and Qt. These base 
       * parser objects do not necessarily have to deal with reading any input lines and 
       * are primarily focused on adding output with the new interfaces provided. 
       * 
       * This class also contains numerous static helper functions used throughout the 
       * C++/Qt parsing classes. Most of these helper functions deal with constructing 
       * strings or list of strings for output. 
       */
      class Base : public AbstractParser
      {
         Q_OBJECT
      public:
         /*!
          * This interface is called to add the comment section of this base parser object 
          * to output. 
          */
         virtual void outputComments() = 0;
         /*!
          * This interface is called to add the declaration of this base parser object to 
          * output. 
          */
         virtual void outputDeclaration() = 0;
         /*!
          * This interface is called to add the definition of this base parser object to 
          * output. 
          */
         virtual void outputDefinition() = 0;
      public:
         static QStringList makeComment(const QString& text, int justified = 0);
         static QStringList makeTemplateComments(const AbstractBlock* block);
         static QString makePreScope(const AbstractBlock* block);
         static QString makePreClassScope(const AbstractBlock* block);
         static QString makeTemplateDeclaration(const AbstractBlock* block);
         static QString makeTemplateArguments(const AbstractBlock* block, bool declarative = false);
         explicit Base(AbstractParser* parent);
      private:
         virtual void makeOutput() override final;
      };
   }
}



#endif
