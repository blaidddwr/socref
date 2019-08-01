#include "gui_persistentdialog.h"
#include <QSettings>



namespace Gui
{






/*!
 * Constructs a new persistent dialog with an optional parent. This new dialog
 * will effectively do nothing unless the save settings method is called before
 * its destruction.
 *
 * @param parent Optional parent for this new persistent dialog.
 */
PersistentDialog::PersistentDialog(QWidget* parent)
   :
   QDialog(parent)
{
}






/*!
 * Constructs a new persistent dialog with the given geometry key and an
 * optional parent.
 *
 * @param geometryKey The optional geometry key used as the Qt settings key to
 *                    store the geometry of a unique dialog class. If this
 *                    string is empty it is ignored and no geometry is set.
 *
 * @param parent Optional parent for this new persistent dialog.
 */
PersistentDialog::PersistentDialog(const QString& geometryKey, QWidget* parent)
   :
   QDialog(parent)
{
   saveSettings(geometryKey);
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
   if ( !_geometryKey.isEmpty() )
   {
      // Save the geometry for this dialog.
      QSettings settings;
      settings.setValue(_geometryKey,saveGeometry());
   }

   // Call the parent interface.
   QDialog::closeEvent(event);
}






/*!
 * Sets the geometry key for this persistent dialog and then restores the
 * geometry of this dialog using the given geometry key. The geometry key for
 * this dialog cannot already be set.
 *
 */
void PersistentDialog::saveSettings(const QString& geometryKey)
{
   // Make sure the given geometry key is not empty.
   Q_ASSERT(!geometryKey.isEmpty());

   // Make sure the geometry key for this dialog is not already set.
   Q_ASSERT(_geometryKey.isEmpty());

   // Set the geometry key for this dialog and restore its geometry using the same
   // key.
   _geometryKey = geometryKey;
   QSettings settings;
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}

}
