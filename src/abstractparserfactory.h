#ifndef ABSTRACTPARSERFACTORY_H
#define ABSTRACTPARSERFACTORY_H
#include <memory>
#include <QObject>
#include "abstractparser.h"
#include "global.h"
//



/*!
 */
class AbstractParserFactory : public QObject
{
   Q_OBJECT
public:
   /*!
    *
    * @param name  
    *
    * @param extension  
    */
   virtual std::unique_ptr<AbstractParser> make(const QString& name, const QString& extension) = 0;
};



#endif
