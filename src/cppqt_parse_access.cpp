#include "cppqt_parse_access.h"
#include "cppqt_access.h"



using namespace CppQt::Parse;






Access::Access(CppQt::Access* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






void Access::outputComments()
{}






void Access::outputDeclaration()
{
   setIndent(indent() - 3);
   add(CppQt::Access::_typeNames.at(static_cast<int>(_block->accessType())));
   setIndent(indent() + 3);
}






void Access::outputDefinition()
{}






bool Access::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
