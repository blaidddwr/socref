/*@ version 1 @*/
#ifndef SHAPE_H
#define SHAPE_H
#include <QString>
#include "Global.h"
namespace Shape {

class Route
{
public:
    QString path;
    AbstractBlock* block;
    int parseIndex;
    int builderIndex;
};

}

/*!
 * @namespace Shape
 *
 * This contains all shapes.
 *
 * Shapes are simple C structure classes whose members are all public and have
 * default constructor and destructor methods.
 */

/*!
 * @class Shape::Route
 *
 * This is a shape class. It holds all information for a single source code file
 * route used by the parsing and building system.
 */

#endif
