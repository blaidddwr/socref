#include "gui_persistentdialog.h"
#include <QSettings>
#include "application.h"
#include "exception.h"



using namespace Gui;
//






/*!
 * Constructs a new persistent dialog with the optional geometry key and parent. If 
 * no geometry key is given this new dialog will effectively do nothing unless the 
 * save settings method is called before its destruction. 
 *
 * @param geometryKey The optional geometry key used as the Qt settings key to 
 *                    store the geometry of a unique dialog class. 
 *
 * @param parent Optional parent for this new persistent dialog. 
 */
PersistentDialog::PersistentDialog(const char* geometryKey, QWidget* parent):
   QDialog(parent)
{
   if ( geometryKey ) saveSettings(geometryKey);
}






/*!
 * Saves the geometry of this persistent dialog if the geometry key has been set, 
 * else this does nothing. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's geometry key has been set then use Qt settings to save this 
 *    object's dialog geometry using the geometry key. 
 */
PersistentDialog::~PersistentDialog()
{
   if ( _geometryKey )
   {
      QSettings settings(Application::_companyKey,Application::_programKey);
      settings.setValue(_geometryKey,saveGeometry());
   }
}






/*!
 * Sets the geometry key for this persistent dialog and then restores the geometry 
 * of this dialog using the given geometry key. If the geometry key for this dialog 
 * has already been set then an exception is thrown. 
 *
 * @param geometryKey  
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's geometry key is not null then throw an exception. 
 *
 * 2. Set this object's geometry key to the one given and then restore the geometry 
 *    of this persistent dialog using the given geometry key. 
 */
void PersistentDialog::saveSettings(const char* geometryKey)
{
   // 1
   if ( _geometryKey )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The geometry key for this persistent dialog has already been set."));
      throw e;
   }

   // 2
   _geometryKey = geometryKey;
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
