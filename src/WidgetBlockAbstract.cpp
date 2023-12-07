#include "WidgetBlockAbstract.h"
#include "BlockAbstract.h"
namespace Widget {
namespace Block {


Abstract::Abstract(
    ::Block::Abstract* block
    ,QWidget* parent
):
    QWidget(parent)
    ,_block(block)
{
    Q_ASSERT(block);
    connect(block,&QObject::destroyed,this,&Abstract::onBlockDestroyed);
}


::Block::Abstract* Abstract::block(
) const
{
    Q_ASSERT(_block);
    return _block;
}


void Abstract::onBlockDestroyed(
    QObject* object
)
{
    if (_block == object)
    {
        _block = nullptr;
    }
}
}
}
