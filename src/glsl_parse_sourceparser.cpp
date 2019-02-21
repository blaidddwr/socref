#include "glsl_parse_sourceparser.h"



namespace GLSL
{
namespace Parse
{






/*!
 * Constructs a new source parser.
 */
SourceParser::SourceParser()
   :
   Basic::InputParser("\\A#","\\A\\z",true)
{
}

}
}
