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
         explicit Function(int indent, CppQt::Function* block);
         explicit Function(int indent, const QString& header);
      protected:
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
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
         void outputComment(const QString& text, int indent = 0);
         CppQt::Function* _block {nullptr};
         QString _header;
         int _baseIndent;
         int _level {0};
         QStringList _code;
         QStringList _initializers;
      };
   }
}



#endif
