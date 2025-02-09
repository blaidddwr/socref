#include "ControllerCodeAbstractParser.h"
namespace Controller {
namespace Code {


AbstractParser::AbstractParser(
    AbstractParser* parent
):
    QObject(parent)
{
}


const QList<AbstractParser*>& AbstractParser::children(
)
{
    return _children;
}


void AbstractParser::setBlock(
    Block::Abstract* object
)
{
    _block = object;
}


void AbstractParser::setVersion(
    int value
)
{
    _version = value;
}


int AbstractParser::version(
) const
{
    Q_ASSERT(_version != -1);
    return _version;
}


void AbstractParser::addChild(
    AbstractParser* child
)
{
    Q_ASSERT(child);
    child->setParent(this);
    _children.append(child);
}
}
}
