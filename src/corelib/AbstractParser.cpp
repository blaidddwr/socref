#include "AbstractParser.h"
#include "AbstractBlock.h"
#include "Exception.h"


AbstractParser::AbstractParser(
    AbstractBlock* block
    ,int version
    ,QObject* parent
):
    QObject(parent)
    ,_block(block)
    ,_version(version)
{
    Q_ASSERT(block);
    connect(block,&QObject::destroyed,this,&AbstractParser::onBlockDestroyed);
    if (auto p = qobject_cast<AbstractParser*>(parent))
    {
        p->_children.append(this);
        connect(this,&QObject::destroyed,p,&AbstractParser::onChildDestroyed);
    }
}


AbstractBlock* AbstractParser::block(
) const
{
    return _block;
}


AbstractParser* AbstractParser::get(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _children.size());
    return _children.at(index);
}


int AbstractParser::size(
) const
{
    return _children.size();
}


int AbstractParser::version(
) const
{
    return _version;
}


void AbstractParser::insertCode(
    const QString& key
    ,const QStringList& lines
    ,AbstractBlock* block
)
{
    using LogicalParse = Exception::LogicalParse;
    if (!block)
    {
        Q_ASSERT(_block);
        block = _block;
    }
    if (!lines.isEmpty())
    {
        auto& code = block->code();
        if (code.contains(key))
        {
            throw LogicalParse(tr("Code key %1 collision in block %2.").arg(key,block->label()));
        }
        code.insert(key,lines);
    }
}


void AbstractParser::onBlockDestroyed(
    QObject* object
)
{
    if (_block == object)
    {
        throw std::logic_error("block destroyed during lifetime of abstract parser");
    }
}


void AbstractParser::onChildDestroyed(
    QObject* object
)
{
    _children.removeOne(object);
}
