#ifndef ABSTRACTPARSERFACTORY_H
#define ABSTRACTPARSERFACTORY_H
#include <memory>
#include <QObject>
#include "abstractparser.h"
#include "global.h"
//



/*!
 * This represents the factory that can produce parser objects for a specific 
 * project type. It is a very simple class that has only one interface for 
 * producing new parser objects to parse individual source files. 
 */
class AbstractParserFactory : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface makes a new parser object from the given file and and 
    * extension. This can return a null pointer in which case the source file is 
    * ignored and skipped. 
    *
    * @param name Name of the source file that is parsed with the returned parser 
    *             object. 
    *
    * @param extension Extension of the file that is parsed with the returned 
    *                  parser object. 
    *
    * @return Pointer to new parser object or null pointer. 
    */
   virtual std::unique_ptr<AbstractParser> make(const QString& name, const QString& extension) const = 0;
};



#endif
