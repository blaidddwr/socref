#ifndef CPPQT_PARSE_MAIN_H
#define CPPQT_PARSE_MAIN_H
#include "cppqt_parse_source.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       * This is the main parser. This parses the special main source file that only 
       * contains the main function. Because this is a special source file this inherits 
       * from the source class. Because this parser is only responsible for the single 
       * main function the namespace block it is given must be the root block of its 
       * project. 
       */
      class Main : public Source
      {
         Q_OBJECT
      public:
         explicit Main(const Namespace* block);
      protected:
         virtual void makeOutput() override final;
         virtual void evaluateVariable(CppQt::Variable* block) override final;
         virtual void evaluateFunction(CppQt::Function* block) override final;
         virtual void evaluateOther(AbstractBlock* block) override final;
      private:
         /*!
          * True if this parser found the main function block and added it to its 
          * definitions or false otherwise. This is used so only a single main function is 
          * added to its definitions. 
          */
         bool _found {false};
      };
   }
}



#endif
