#include "cppqt_parse_commentparser.h"
#include "cppqt_settings.h"



namespace CppQt
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
