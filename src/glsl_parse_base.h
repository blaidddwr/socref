#ifndef GLSL_PARSE_BASE_H
#define GLSL_PARSE_BASE_H
#include "abstractparser.h"
//



namespace GLSL
{
   namespace Parse
   {
      /*!
       * This is the base parser that all other GLSL parser classes use for common 
       * functionality. Currently the only common functionality is adding comments. 
       */
      class Base : public AbstractParser
      {
         Q_OBJECT
      public:
         using AbstractParser::AbstractParser;
         virtual void outputLines();
         virtual void outputDefinition();
      protected:
         virtual void makeOutput() override;
      protected:
         void addComment(const QString& text, int justified = 0);
      };
   }
}



#endif
