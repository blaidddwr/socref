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
       */
      class Function : public Base
      {
         Q_OBJECT
      public:
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
      public:
         Function(CppQt::Function* block, AbstractParser* parent);
         Function(const QString& definition, AbstractParser* parent);
         bool isMatch(const QString& line);
         bool hasCode() const;
         void setCutOff(int cutOff);
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         void processInlineComment(const QString& line);
         void insertInlineComment(int index, int spacing);
         void outputArgumentComments();
         void outputReturnDescriptionComment();
         QString getReturnValue();
         QString getScope(bool hasTemplates);
         QString getName(bool isRegExp = false);
         QString getArguments(bool withInitializers);
         bool hasAnyTemplates();
         /*!
          */
         CppQt::Function* _block {nullptr};
         /*!
          */
         QString _definition;
         /*!
          */
         int _level {0};
         /*!
          */
         int _cutOff {0};
         /*!
          */
         bool _edgePast {false};
         /*!
          */
         int _nextOperation {0};
         /*!
          */
         QStringList _code;
         /*!
          */
         QStringList _initializers;
      };
   }
}



#endif
