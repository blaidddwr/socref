#include "gui_persistentdialog.h"
#include <QSettings>
#include "application.h"






PersistentDialog::PersistentDialog(const char* geometryKey, QWidget* parent):
   QDialog(parent),
   _geometryKey(geometryKey)
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}






PersistentDialog::~PersistentDialog()
{
   QSettings settings(Application::_companyKey,Application::_programKey);
   settings.setValue(_geometryKey,saveGeometry());
}
