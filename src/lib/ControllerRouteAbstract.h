#ifndef CONTROLLER_ROUTE_ABSTRACT_H
#define CONTROLLER_ROUTE_ABSTRACT_H
#include <QObject>
#include "Block.h"
#include "ControllerParse.h"
#include "ShapeRoute.h"
namespace Controller {
namespace Route {




/*!
 * This is an abstract class. It is a route controller for a language
 * implementation. A router provides routes to all source code files for a
 * project's root block.
 */
class Abstract:
    public QObject
{


    public:
    Abstract(
        QObject* parent = nullptr
    );


    /*!
     * Generates all routes for a given root block.
     *
     * @param root
     *        The root block.This must be valid, have the same language
     *        implementation as this router, and the root block of a project.
     *
     * @return
     * All routes for a given root block.
     */
    public:
    virtual QList<Shape::Route> routes(
        const Block::Abstract* root
    ) = 0;
};
}
}


#endif
