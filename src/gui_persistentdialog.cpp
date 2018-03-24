#include "gui_persistentdialog.h"
#include <QSettings>
#include "application.h"



using namespace Gui;






PersistentDialog::PersistentDialog(const char* geometryKey, QWidget* parent):
   QDialog(parent)
{
   if ( geometryKey ) saveSettings(geometryKey);
}






PersistentDialog::~PersistentDialog()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
}






void PersistentDialog::saveSettings(const char* geometryKey)
{
   _geometryKey = geometryKey;
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
