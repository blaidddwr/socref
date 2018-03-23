#ifndef DOMELEMENTREADER_MATCH_H
#define DOMELEMENTREADER_MATCH_H
#include "domelementreader.h"
//



/*!
 * This holds a single match for an XML element reader. 
 */
class DomElementReader::Match
{
public:
   /*!
    * The type of match this is. 
    */
   Type _type;
   /*!
    * The pointer to the variable for this match. 
    */
   void* _data;
   /*!
    * The read state of this match. True if this data has been read otherwise 
    * false. 
    */
   bool _read;
};



#endif
