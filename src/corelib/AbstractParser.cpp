#include "AbstractParser.h"


AbstractParser::AbstractParser(
    AbstractParser* parent
    ,AbstractBlock* block
    ,int version
):
    QObject(parent)
    ,_block(block)
    ,_version(version)
{
}


const QList<AbstractParser*>& AbstractParser::children(
)
{
    return _children;
}


void AbstractParser::reset(
)
{
    for (auto child: std::as_const(_children))
    {
        child->reset();
    }
}


void AbstractParser::setBlock(
    AbstractBlock* object
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


AbstractBlock* AbstractParser::block(
) const
{
    Q_ASSERT(_block);
    return _block;
}
