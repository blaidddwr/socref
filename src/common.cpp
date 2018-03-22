#include "common.h"
#include <QMessageBox>
//






/*!
 *
 * @param text  
 *
 * @param exception  
 */
void showException(const QString& text, const Exception::Base& exception)
{
   QMessageBox info;
   info.setWindowTitle(exception.title());
   info.setText(text);
   info.setInformativeText(exception.details());
   info.setIcon(QMessageBox::Warning);
   info.exec();
}
