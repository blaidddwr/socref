#include "CppQtLanguage.h"
#include <QtGui>
#include "CppBlock.h"
#include "CppQtBlock.h"
#include "CppQtBlockFunction.h"
#include "ModelMetaBlock.h"
namespace CppQt {
using namespace Cpp::Block;
using CppQtFunction = CppQt::Block::Function;


Language::Language(
):
    Cpp::Language(new Model::Meta::Language("cppqt","C++/Qt",QIcon(":/cppqt.svg")))
{
    CppQtFunction::initializeIcons();
}


AbstractBlock* Language::createBlock(
    int index
    ,QObject* parent
) const
{
    if (index == FunctionIndex)
    {
        auto meta = blockMeta(index);
        Q_ASSERT(meta->index() == FunctionIndex);
        return new CppQtFunction(meta,parent);
    }
    else
    {
        return Cpp::Language::createBlock(index,parent);
    }
}
}
