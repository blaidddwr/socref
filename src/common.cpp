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






QDomElement makeElement(QDomDocument& document, const QString& tagName, const QString& text)
{
   QDomElement ret {document.createElement(tagName)};
   ret.appendChild(document.createTextNode(text));
   return ret;
}






QDomElement makeElement(QDomDocument& document, const QString& tagName, int value)
{
   QDomElement ret {document.createElement(tagName)};
   ret.appendChild(document.createTextNode(QString::number(value)));
   return ret;
}
