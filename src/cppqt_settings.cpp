#include "cppqt_settings.h"
#include <QSettings>
#include "application.h"



using namespace CppQt;
const char* Settings::_indentSpacesKey {"settings.cppqt.indent.spaces"};
const char* Settings::_headerLinesKey {"settings.cppqt.header.lines"};
const char* Settings::_functionLinesKey {"settings.cppqt.header.functionlines"};
const char* Settings::_maxColumnsKey {"settings.cppqt.header.maxcolumns"};
//






Settings::Settings()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   _indentSpaces = settings.value(_indentSpacesKey).toInt();
   _headerLines = settings.value(_headerLinesKey).toInt();
   _functionLines = settings.value(_functionLinesKey).toInt();
   _maxColumns = settings.value(_maxColumnsKey).toInt();
}






int Settings::indentSpaces() const
{
   return _indentSpaces;
}






int Settings::headerLines() const
{
   return _headerLines;
}






int Settings::functionLines() const
{
   return _functionLines;
}






int Settings::maxColumns() const
{
   return _maxColumns;
}






void Settings::setIndentSpaces(int indentSpaces)
{
   _indentSpaces = indentSpaces;
   updateSetting(_indentSpacesKey,_indentSpaces);
}






void Settings::setHeaderLines(int headerLines)
{
   _headerLines = headerLines;
   updateSetting(_headerLinesKey,_headerLines);
}






void Settings::setFunctionLines(int functionLines)
{
   _functionLines = functionLines;
   updateSetting(_functionLinesKey,_functionLines);
}






void Settings::setMaxColumns(int maxColumns)
{
   _maxColumns = maxColumns;
   updateSetting(_maxColumnsKey,_maxColumns);
}






void Settings::updateSetting(const QString& key, int value)
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(key,value);
}
