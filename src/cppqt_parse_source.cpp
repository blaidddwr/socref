#include "cppqt_parse_source.h"



using namespace CppQt::Parse;
//






/*!
 */
Source::Source():
   Basic::InputParser("\\A#include\\s+\"[a-z_]+\\.h\"\\z","\\A\\z")
{}
