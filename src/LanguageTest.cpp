#include "LanguageTest.h"
#include <QtGui>
#include "Exception.h"
#include "BlockTest.h"
#include "BlockTestNode.h"
#include "ModelMetaBlock.h"
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


int Test::rootIndex(
) const
{
    return NodeIndex;
}
}
