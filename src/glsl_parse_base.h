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
         /*!
          * Constructs a new root parser base object. 
          */
         explicit Base() = default;
         explicit Base(AbstractParser* parent);
      protected:
         void addComment(const QString& text, int justified = 0);
      };
   }
}



#endif
