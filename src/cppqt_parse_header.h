#ifndef CPPQT_PARSE_HEADER_H
#define CPPQT_PARSE_HEADER_H
#include "basic_inputparser.h"
//



namespace CppQt
{
   namespace Parse
   {
      /*!
       */
      class Header : public Basic::InputParser
      {
         Q_OBJECT
      public:
         explicit Header();
      };
   }
}



#endif
