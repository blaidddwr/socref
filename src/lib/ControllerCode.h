#ifndef CONTROLLER_CODE_H
#define CONTROLLER_CODE_H
namespace Controller {


/*!
 * Detailed description.
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
