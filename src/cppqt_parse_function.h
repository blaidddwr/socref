#ifndef CPPQT_PARSE_FUNCTION_H
#define CPPQT_PARSE_FUNCTION_H
#include "cppqt_parse_base.h"



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
         bool isMatch(QString line);
         bool hasCode() const;
         virtual void makeOutput() override final;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         void outputComments();
         void outputArgumentComments();
         void outputReturnDescriptionComment();
         void outputOperationComments();
         CppQt::Function* _block {nullptr};
         QString _definition;
         int _level {0};
         QStringList _code;
         QStringList _initializers;
      };
   }
}



#endif
