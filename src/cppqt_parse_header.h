#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
#include "cppqt_parse_source.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
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
         void outputHeader();
         void outputDeclarations();
         void outputClassComments(const Class* block);
         void outputClassDeclaration(const Class* block);
         void outputFooter();
         /*!
          */
         const Namespace* _block;
         /*!
          */
         int _indentSpaces;
         /*!
          */
         int _headerLines;
         /*!
          */
         QList<Base*> _declarations;
         /*!
          */
         QString _header1;
         /*!
          */
         QString _header2;
      };
   }
}



#endif
