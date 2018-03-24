#include "cppqt_parse_access.h"
#include "cppqt_gui_settingsdialog.h"
#include "cppqt_access.h"



using namespace CppQt::Parse;






Access::Access(CppQt::Access* block, AbstractParser* parent):
   Base(parent),
   _block(block),
   _indentSpaces(Gui::SettingsDialog::indentSpaces())
{}






void Access::outputComments()
{}






void Access::outputDeclaration()
{
   setIndent(indent() - _indentSpaces);
   add(CppQt::Access::_typeNames.at(static_cast<int>(_block->accessType())));
   setIndent(indent() + _indentSpaces);
}






void Access::outputDefinition()
{}






bool Access::readLine(const QString& line)
{
   Q_UNUSED(line)
   return false;
}
