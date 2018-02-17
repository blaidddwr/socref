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
         explicit Global(Namespace* root);
      protected:
         virtual bool readLine(const QString& line) override final;
         virtual void makeOutput() override final;
      private:
         Namespace* _root;
      };
   }
}



#endif
