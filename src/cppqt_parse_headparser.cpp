#include "cppqt_parse_headparser.h"



namespace CppQt
{
namespace Parse
{






/*!
 * Constructs a new header parser element.
 */
HeadParser::HeadParser()
   :
   Basic::InputParser("\\A#define\\s+[a-zA-Z_]*_H\\z","\\A\\z")
{
}

}
}
