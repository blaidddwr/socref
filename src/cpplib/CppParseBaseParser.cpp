#include "CppParseBaseParser.h"
#include "Cpp.h"
#include "CppBlockBase.h"
#include "Exception.h"
namespace Cpp {
namespace Parse {
using namespace Cpp::Block;


BaseParser::BaseParser(
    AbstractParser* parent
    ,AbstractBlock* block
    ,int version
):
    AbstractParser(parent,block,version)
{
}


void BaseParser::insertCode(
    int keyIndex
    ,const QStringList& lines
)
{
    using LogicalParse = Exception::LogicalParse;
    Q_ASSERT(block());
    const auto& key = codeKey(keyIndex);
    auto& code = block()->code();
    if (code.contains(key))
    {
        QStringList labels;
        auto b = qobject_cast<Base*>(block());
        Q_ASSERT(b);
        auto pb = qobject_cast<Base*>(b->parent());
        while (pb)
        {
            labels.prepend(b->name());
            b = pb;
            pb = qobject_cast<Base*>(b->parent());
        }
        throw LogicalParse(
            tr("While parsing block %1, multiple instances of code with the key %2 were parsed."
               " This would cause the first instance of code to be overwritten by the second,"
               " causing the first instance to be lost when the source code is built.")
                .arg(labels.join("::"),key)
            );
    }
    code.insert(key,lines);
}
}
}
