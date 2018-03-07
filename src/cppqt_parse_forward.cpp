#include "cppqt_parse_forward.h"
#include "cppqt_class.h"



using namespace CppQt::Parse;






Forward::Forward(Class* block, AbstractParser* parent):
   Base(parent),
   _block(block)
{}






void Forward::outputComments()
{}






void Forward::outputDeclaration()
{
   addLine(QString("class ").append(_block->Base::name()).append(";"));
}






void Forward::outputDefinition()
{}






bool Forward::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
