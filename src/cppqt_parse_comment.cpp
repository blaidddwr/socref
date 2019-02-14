#include "cppqt_parse_comment.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 */
Comment::Comment():
   Basic::CommentParser("/*!"," *"," */",Settings::instance().maxColumns())
{}
