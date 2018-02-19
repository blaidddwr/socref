#ifndef CPPQT_PARSE_FUNCTION_H
#define CPPQT_PARSE_FUNCTION_H
#include "cppqt_parse_base.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Function : public Base
      {
         Q_OBJECT
      public:
         explicit Function(CppQt::Function* block, AbstractParser* parent);
         explicit Function(const QString& header, AbstractParser* parent);
         virtual void outputDetachedComments() override final;
         virtual void outputComments() override final;
         virtual void outputDeclaration() override final;
         virtual void outputDefinition() override final;
         bool isMatch(const QString& line);
         bool hasCode() const;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         void outputArgumentComments();
         void outputReturnDescriptionComment();
         void outputOperationComments();
         QString getReturnValue();
         QString getScope(bool hasTemplates);
         QString getName();
         QString getArguments(bool withInitializers);
         bool hasAnyTemplates();
         CppQt::Function* _block {nullptr};
         QString _definition;
         int _level {0};
         QStringList _code;
         QStringList _initializers;
      };
   }
}



#endif
