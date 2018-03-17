#ifndef COMMON_H
#define COMMON_H
#include <QDomElement>
#include <exception.h>
//



class AbstractBlock;
class AbstractBlockFactory;
class AbstractParser;
class AbstractParserFactory;
class AbstractProjectFactory;
class ProjectFactory;
class BlockModel;
class ScanThread;
class Project;
void showException(const QString& text, const Exception::Base& exception);
QDomElement makeElement(QDomDocument& document, const QString& tagName, const QString& text);
QDomElement makeElement(QDomDocument& document, const QString& tagName, int value);



#endif
