#include "cppqt_parse_function.h"
#include "cppqt_settings.h"



namespace CppQt
{
namespace Parse
{






/*!
 *
 * @param header The header or full name of this function that will be used as
 *               the first line of output for this function parser.
 *
 * @param operations A list of operation step descriptions this function parser
 *                   uses to generate inline comment block lines.
 */
Function::Function(const QString& header, const QStringList& operations)
   :
   Basic::FunctionParser('{','}',"\"'","// ",Settings::instance().maxColumns(),header,operations)
{
}

}
}
