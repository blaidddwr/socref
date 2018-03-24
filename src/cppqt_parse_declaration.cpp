#include "cppqt_parse_declaration.h"
#include "cppqt_declaration.h"



using namespace CppQt::Parse;






Declaration::Declaration(CppQt::Declaration* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






void Declaration::outputComments()
{}






void Declaration::outputDeclaration()
{
   add(_block->declareTypeName().append(" ").append(_block->field()).append(";"));
}






void Declaration::outputDefinition()
{}






bool Declaration::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
