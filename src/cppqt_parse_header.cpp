#include "cppqt_parse_header.h"



namespace CppQt
{
namespace Parse
{






/*!
 * Constructs a new header parser element.
 */
Header::Header()
   :
   Basic::InputParser("\\A#define\\s+[a-zA-Z_]*_H\\z","\\A\\z")
{
}

}
}
