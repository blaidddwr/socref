#ifndef COMMON_H
#define COMMON_H
#include <QDomElement>
#include <exception.h>
//



void showException(const QString& text, const Exception::Base& exception);
QDomElement makeElement(QDomDocument& document, const QString& tagName, const QString& text);
QDomElement makeElement(QDomDocument& document, const QString& tagName, int value);



#endif
