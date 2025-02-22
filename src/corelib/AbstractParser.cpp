#include "AbstractParser.h"
#include "AbstractBlock.h"
#include "Exception.h"


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


void AbstractParser::insertCode(
    const QString& key
    ,const QStringList& lines
)
{
    using LogicalParse = Exception::LogicalParse;
    Q_ASSERT(block());
    if (!lines.isEmpty())
    {
        auto& code = block()->code();
        if (code.contains(key))
        {
            throw LogicalParse(tr("Code key collision in block %1.").arg(block()->displayText()));
        }
        code.insert(key,lines);
    }
}
