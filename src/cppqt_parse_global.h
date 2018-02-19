#ifndef CPPQT_PARSE_GLOBAL_H
#define CPPQT_PARSE_GLOBAL_H
#include "abstractparser.h"
#include "cppqt.h"



namespace CppQt
{
   namespace Parse
   {
      class Global : public AbstractParser
      {
         Q_OBJECT
      public:
         explicit Global(Namespace* block);
      protected:
         virtual bool readLine(const QString& line) override;
         virtual void makeOutput() override;
         void beginNamespaceNesting();
         void endNamespaceNesting();
      private:
         Namespace* _block;
         int _depth {0};
      };
   }
}



#endif
