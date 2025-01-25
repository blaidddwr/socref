#ifndef CONTROLLER_ROUTE_ABSTRACT_H
#define CONTROLLER_ROUTE_ABSTRACT_H
#include <QObject>
#include "Block.h"
namespace Controller {
namespace Route {




/*!
 * This is an abstract class. It is a route controller for a language
 * implementation. A router provides routes to all source code files for a given
 * project's root block.
 */
class Abstract:
    public QObject
{


    /*!
     * Returns all source code routes for the project with the given root block.
     *
     * @param root
     *        The root block. It must be valid and have the same language
     *        implementation as this router.
     *
     * @return
     * All source code routes. The keys are the relative file path to the source
     * code, where each path is relative to the given root block's project's
     * source path. The values are the parse indexes used to parse the
     * corresponding source code.
     */
    public:
    virtual QHash<QString,int> routes(
        const Block::Abstract* root
    ) = 0;
};
}
}


#endif
