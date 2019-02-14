#include "cppqt_parse_source.h"



using namespace CppQt::Parse;
//






/*!
 */
Source::Source():
   Basic::InputParser("\\A#include\\s+\"[a-zA-Z_]+\\.h\\z","\\A\\z")
{}
