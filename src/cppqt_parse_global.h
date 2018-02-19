#ifndef CPPQT_PARSE_GLOBAL_H
#define CPPQT_PARSE_GLOBAL_H
#include "cppqt_parse_base.h"



namespace CppQt
{
   namespace Parse
   {
      class Global : public Base
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
