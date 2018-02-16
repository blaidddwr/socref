#ifndef ABSTRACTPARSERFACTORY_H
#define ABSTRACTPARSERFACTORY_H
#include <memory>
#include <QObject>
#include "abstractparser.h"
#include "global.h"



class AbstractParserFactory : public QObject
{
   Q_OBJECT
public:
   virtual std::unique_ptr<AbstractParser> makeParser(const QString& name, const QString& extension) const = 0;
};



#endif
