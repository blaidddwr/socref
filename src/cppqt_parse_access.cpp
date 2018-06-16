#include "cppqt_parse_access.h"
#include "cppqt_access.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;






Access::Access(CppQt::Access* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _indentSpaces(Settings::instance().indentSpaces())
{}






void Access::outputComments()
{}






void Access::outputDeclaration()
{
   setIndent(indent() - _indentSpaces);
   add(_block->accessTypeString());
   setIndent(indent() + _indentSpaces);
}






void Access::outputDefinition()
{}






bool Access::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
