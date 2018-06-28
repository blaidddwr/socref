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
   // If the geometry key is not null then save the geometry of this dialog. 
   if ( geometryKey ) saveSettings(geometryKey);
}






/*!
 * Implements _QDialog_ interface. This is implemented so when done is called it 
 * closes this dialog. 
 *
 * @param r See Qt docs. 
 */
void PersistentDialog::done(int r)
{
   // Call the qt dialog interface and then close this dialog. 
   QDialog::done(r);
   close();
}






/*!
 * Implements _QWidget::closeEvent_. This saves the geometry of this persistent 
 * dialog and calls the inherited interface. 
 *
 * @param event See Qt docs. 
 */
void PersistentDialog::closeEvent(QCloseEvent* event)
{
   // Check to see if this dialog has a geometry key. 
   if ( _geometryKey )
   {
      // Save the geometry for this dialog. 
      QSettings settings(Application::_companyKey,Application::_programKey);
      settings.setValue(_geometryKey,saveGeometry());
   }

   // Call the parent interface. 
   QDialog::closeEvent(event);
}






/*!
 * Sets the geometry key for this persistent dialog and then restores the geometry 
 * of this dialog using the given geometry key. If the geometry key for this dialog 
 * has already been set then an exception is thrown. 
 *
 * @param geometryKey  
 */
void PersistentDialog::saveSettings(const char* geometryKey)
{
   // Make sure the geometry key for this dialog is not already set. 
   if ( _geometryKey )
   {
      Exception::LogicError e;
      MARK_EXCEPTION(e);
      e.setDetails(tr("The geometry key for this persistent dialog has already been set."));
      throw e;
   }

   // Set the geometry key for this dialog and restore its geometry using the same 
   // key. 
   _geometryKey = geometryKey;
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
