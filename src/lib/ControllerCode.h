#ifndef CONTROLLER_CODE_H
#define CONTROLLER_CODE_H
namespace Controller {


/*!
 * This contains all code controllers.
 * 
 * Code controllers handle everything related to parsing and building source
 * code files to and from a project.
 */
namespace Code
{
    class AbstractParser;
    class AbstractRouter;
    class Make;


    /*!
     * This enumerates the legacy code version used by all language
     * implementations for parsing and building.
     */
    enum Version
    {
        Code_Legacy = 0
    };
}
}


#endif
