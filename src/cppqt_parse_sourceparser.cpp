#include "cppqt_parse_sourceparser.h"



namespace CppQt
{
namespace Parse
{






/*!
 * Constructs a new source parser with the optional setting of including the
 * first line or not. The first line should only be included for the special
 * main source file.
 *
 * @param first True to set this new source parser to read in the first line of
 *              input or false to set it to ignore the first line.
 */
SourceParser::SourceParser(bool first)
   :
   Basic::InputParser("\\A#include\\s+[\"<][a-zA-Z_\\.]+[a-zA-Z0-9_]*[\">]\\z","\\A\\z",first)
{
}

}
}
