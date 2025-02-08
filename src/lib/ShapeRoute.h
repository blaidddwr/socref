#ifndef SHAPE_ROUTE_H
#define SHAPE_ROUTE_H
#include <QString>
#include "Block.h"
namespace Shape {




/*!
 * This is a shape class. It holds all information for a single source code file
 * route used by the parsing and building system.
 */
class Route
{
    public:
    QString path;
    Block::Abstract* block;
    int parseIndex;
    int builderIndex;
};
}


#endif
