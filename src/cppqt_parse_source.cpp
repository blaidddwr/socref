#include "cppqt_parse_source.h"



using namespace CppQt::Parse;
//






/*!
 *
 * @param first True to set this new source parser to read in the first line of 
 *              input or false to set it to ignore the first line. 
 */
Source::Source(bool first):
   Basic::InputParser("\\A#include\\s+[\"<][a-zA-Z_\\.]+[\">]\\z","\\A\\z",first)
{}
