#ifndef ABSTRACTPARSERFACTORY_H
#define ABSTRACTPARSERFACTORY_H
#include <memory>
#include "abstractparser.h"



class AbstractParserFactory
{
public:
   virtual std::unique_ptr<AbstractParser> makeParser(const QString& name, const QString& extension) const = 0;
};



#endif
