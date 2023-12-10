#include "LanguageTest.h"
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
    appendBlock(new Model::Meta::Block(meta,NodeIndex,"node","Node",{NodeIndex}));
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
        Q_ASSERT(meta->index() == NodeIndex);
        return new Node(meta,parent);
    }
    default:
        ASSERT(false);
        return nullptr;
    }
}


int Test::rootIndex(
) const
{
    return NodeIndex;
}
}
