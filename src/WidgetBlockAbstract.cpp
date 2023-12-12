#include "WidgetBlockAbstract.h"
#include "BlockAbstract.h"
#include "Exception.h"
namespace Widget {
namespace Block {


Abstract::Abstract(
    ::Block::Abstract* block
    ,QWidget* parent
):
    QWidget(parent)
    ,_block(block)
{
    G_ASSERT(block);
    connect(block,&QObject::destroyed,this,&Abstract::onBlockDestroyed);
}


::Block::Abstract* Abstract::block(
) const
{
    G_ASSERT(_block);
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
