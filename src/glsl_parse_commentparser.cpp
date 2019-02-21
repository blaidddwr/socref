#include "glsl_parse_commentparser.h"
#include "glsl_settings.h"



namespace GLSL
{
namespace Parse
{






/*!
 * Constructs a new comment parser element.
 */
CommentParser::CommentParser()
   :
   Basic::CommentParser("/*!"," *"," */",Settings::instance().maxColumns())
{
}

}
}
