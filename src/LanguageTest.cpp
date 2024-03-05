#include "LanguageTest.h"
#include <QtGui>
#include "BlockTest.h"
#include "BlockTestNode.h"
#include "ModelMetaBlock.h"
#include "gassert.h"
namespace Language {
using namespace Block::Test;


Test::Test(
    Model::Meta::Language* meta
    ,QObject* parent
):
    Abstract(meta,parent)
{
    appendBlock(new Model::Meta::Block(meta,NodeIndex,"node","Node",new QIcon(),{NodeIndex}));
}


Block::Abstract* Test::create(
    int index
    ,QObject* parent
) const
{
    switch (index)
    {
    case NodeIndex:
    {
        auto meta = blockMeta(index);
        G_ASSERT(meta->index() == NodeIndex);
        return new Node(meta,parent);
    }
    default:
        G_ASSERT(false);
        return nullptr;
    }
}


Block::Abstract* Test::createRoot(
    QObject* parent
) const
{
    auto ret = new Node(blockMeta(NodeIndex),parent);
    ret->setName(QString());
    return ret;
}


int Test::rootIndex(
) const
{
    return NodeIndex;
}
}
