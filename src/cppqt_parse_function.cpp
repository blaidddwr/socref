#include "cppqt_parse_function.h"
#include "cppqt_settings.h"



using namespace CppQt::Parse;
//






/*!
 *
 * @param operations A list of operation step descriptions this function parser 
 *                   uses to generate inline comment block lines. 
 */
Function::Function(const QStringList& operations):
   Basic::FunctionParser('{','}',"// ",Settings::instance().maxColumns(),operations)
{}
