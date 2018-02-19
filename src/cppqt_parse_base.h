#ifndef CPPQT_PARSE_BASE_H
#define CPPQT_PARSE_BASE_H
#include "abstractparser.h"



namespace CppQt
{
   namespace Parse
   {
      class Base : public AbstractParser
      {
         Q_OBJECT
      public:
         explicit Base(AbstractParser* parent);
         virtual void outputDetachedComments() = 0;
         virtual void outputComments() = 0;
         virtual void outputDeclaration() = 0;
         virtual void outputDefinition() = 0;
      protected:
         virtual void makeOutput() override final;
      };
   }
}



#endif
