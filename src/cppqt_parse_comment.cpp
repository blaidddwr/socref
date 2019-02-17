#include "cppqt_parse_comment.h"
#include "cppqt_settings.h"



namespace CppQt
{
namespace Parse
{






/*!
 */
Comment::Comment()
   :
   Basic::CommentParser("/*!"," *"," */",Settings::instance().maxColumns())
{
}

}
}
