#ifndef CPPQT_PARSE_FUNCTION_H
#define CPPQT_PARSE_FUNCTION_H
#include "abstractparser.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Function : public AbstractParser
      {
         Q_OBJECT
      public:
         explicit Function(CppQt::Function* block, AbstractParser* parent);
         explicit Function(const QString& header, AbstractParser* parent);
         QString header() const;
         bool hasCode() const;
         virtual void makeOutput() override final;
      protected:
         virtual bool readLine(const QString& line) override final;
      private:
         QString buildHeader();
         void appendScope(QString* line);
         void appendClass(QString* line);
         void appendClassTemplate(QString* line, Class* block, bool definition = false);
         void appendNamespace(QString* line);
         void appendArguments(QString* line);
         void outputComments();
         void outputArgumentComments();
         void outputReturnDescriptionComment();
         void outputOperationComments();
         void outputComment(const QString& text, int justified = 0);
         CppQt::Function* _block {nullptr};
         QString _header;
         int _level {0};
         QStringList _code;
         QStringList _initializers;
      };
   }
}



#endif
