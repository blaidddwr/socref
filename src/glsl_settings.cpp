#include "glsl_settings.h"
#include <QSettings>



namespace GLSL
{



/*!
 * The settings key used to save the variable lines setting.
 */
const char* Settings::_variableLinesKey {"glsl.settings.variable.lines"};
/*!
 * The settings key used to save the structure lines setting.
 */
const char* Settings::_structLinesKey {"glsl.settings.struct.lines"};
/*!
 * The settings key used to save the function lines setting.
 */
const char* Settings::_functionLinesKey {"glsl.settings.function.lines"};
/*!
 * The settings key used to save the indent spaces setting.
 */
const char* Settings::_indentSpacesKey {"glsl.indent.spaces"};
/*!
 * The settings key used to save the max columns setting.
 */
const char* Settings::_maxColumnsKey {"glsl.columns.max"};






/*!
 * Constructs a new settings global instance, retrieving all persistent settings
 * from qt settings.
 */
Settings::Settings()
{
   // Create a qt settings object and restore all global settings to their persistent
   // values.
   QSettings settings;
   _variableLines = settings.value(_variableLinesKey).toInt();
   _structLines = settings.value(_structLinesKey).toInt();
   _functionLines = settings.value(_functionLinesKey).toInt();
   _indentSpaces = settings.value(_indentSpacesKey).toInt();
   _maxColumns = settings.value(_maxColumnsKey).toInt();
}






/*!
 * Returns the variable lines setting.
 *
 * @return Variable lines setting.
 */
int Settings::variableLines() const
{
   return _variableLines;
}






/*!
 * Returns the structure lines setting.
 *
 * @return Structure lines setting.
 */
int Settings::structLines() const
{
   return _structLines;
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
 * Returns the indent spaces setting.
 *
 * @return Indent spaces setting.
 */
int Settings::indentSpaces() const
{
   return _indentSpaces;
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
 * Sets the variable lines setting to the given value.
 *
 * @param value The new value for the setting.
 */
void Settings::setVariableLines(int value)
{
   // Update the setting to the new value given.
   _variableLines = value;

   // Update the setting in qt settings for persistence.
   updateSetting(_variableLinesKey,_variableLines);
}






/*!
 * Sets the structure lines setting to the given value.
 *
 * @param value The new value for the setting.
 */
void Settings::setStructLines(int value)
{
   // Update the setting to the new value given.
   _structLines = value;

   // Update the setting in qt settings for persistence.
   updateSetting(_structLinesKey,_structLines);
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
