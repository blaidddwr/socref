#include "cppqt_parse_header.h"



using namespace CppQt::Parse;
//






/*!
 */
Header::Header():
   Basic::InputParser("\\A#define\\s+[a-zA-Z_]*_H\\z","\\A\\z")
{}
