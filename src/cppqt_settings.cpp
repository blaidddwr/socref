#include "cppqt_settings.h"
#include <QSettings>



namespace CppQt
{



/*!
 * The settings key used to save the indent spaces setting.
 */
const QString Settings::_indentSpacesKey {"settings.cppqt.indent.spaces"};
/*!
 * The settings key used to save the header lines setting.
 */
const QString Settings::_headerLinesKey {"settings.cppqt.header.lines"};
/*!
 * The settings key used to save the function lines setting.
 */
const QString Settings::_functionLinesKey {"settings.cppqt.header.functionlines"};
/*!
 * The settings key used to save the max columns setting.
 */
const QString Settings::_maxColumnsKey {"settings.cppqt.header.maxcolumns"};






/*!
 * Constructs a new settings global instance, retrieving all persistent settings
 * from qt settings.
 */
Settings::Settings()
{
   // Create a qt settings object and restore all global settings to their persistent
   // values.
   QSettings settings;
   _indentSpaces = settings.value(_indentSpacesKey).toInt();
   _headerLines = settings.value(_headerLinesKey).toInt();
   _functionLines = settings.value(_functionLinesKey).toInt();
   _maxColumns = settings.value(_maxColumnsKey).toInt();
}






/*!
 * Returns the indent spaces setting.
 *
 * @return Indent spaces setting.
 */
int Settings::indentSpaces() const
{
   return _indentSpaces;
}






/*!
 * Returns the header lines setting.
 *
 * @return Header lines setting.
 */
int Settings::headerLines() const
{
   return _headerLines;
}






/*!
 * Returns the function lines setting.
 *
 * @return Function lines setting.
 */
int Settings::functionLines() const
{
   return _functionLines;
}






/*!
 * Returns the max columns setting.
 *
 * @return Max columns setting.
 */
int Settings::maxColumns() const
{
   return _maxColumns;
}






/*!
 * Sets the indent spaces setting to the given value.
 *
 * @param value The new value for the setting.
 */
void Settings::setIndentSpaces(int value)
{
   // Update the setting to the new value given.
   _indentSpaces = value;

   // Update the setting in qt settings for persistence.
   updateSetting(_indentSpacesKey,_indentSpaces);
}






/*!
 * Sets the header lines setting to the given value.
 *
 * @param value The new value for the setting.
 */
void Settings::setHeaderLines(int value)
{
   // Update the setting to the new value given.
   _headerLines = value;

   // Update the setting in qt settings for persistence.
   updateSetting(_headerLinesKey,_headerLines);
}






/*!
 * Sets the function lines setting to the given value.
 *
 * @param value The new value for the setting.
 */
void Settings::setFunctionLines(int value)
{
   // Update the setting to the new value given.
   _functionLines = value;

   // Update the setting in qt settings for persistence.
   updateSetting(_functionLinesKey,_functionLines);
}






/*!
 * Sets the max columns setting to the given value.
 *
 * @param value The new value for the setting.
 */
void Settings::setMaxColumns(int value)
{
   // Update the setting to the new value given.
   _maxColumns = value;

   // Update the setting in qt settings for persistence.
   updateSetting(_maxColumnsKey,_maxColumns);
}






/*!
 * Updates the given setting to the given value using qt settings so it is
 * persistent.
 *
 * @param key The key where the given value is saved.
 *
 * @param value The value that is saved to the given key.
 */
void Settings::updateSetting(const QString& key, int value)
{
   // Create a qt settings object and save the given value to the given key.
   QSettings settings;
   settings.setValue(key,value);
}

}
