#include "cppqt_parse_comment.h"
#include "cppqt_settings.h"



namespace CppQt
{
namespace Parse
{






/*!
 * Constructs a new comment parser element.
 */
Comment::Comment()
   :
   Basic::CommentParser("/*!"," *"," */",Settings::instance().maxColumns())
{
}

}
}
