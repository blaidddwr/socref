#include "CppParseUnionParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockUnion.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;


UnionParser::UnionParser(
    AbstractBlock* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
    Q_ASSERT(block);
    Q_ASSERT(block->meta()->index() == NamespaceIndex);
    populate();
}


Status UnionParser::parse(
    const QStringList& lines
    ,int where
)
{
    switch (version())
    {
    case Cpp_Legacy:
    case Cpp_1:
        return parseLegacy(lines,where);
    default:
        throw std::logic_error("unknown version");
    }
}


void UnionParser::find(
    const QString& name
)
{
    using LogicalParse = ::Exception::LogicalParse;
    auto i = _unions.find(name);
    if (i == _unions.end())
    {
        throw LogicalParse(tr("Encountered unknown union name %1.").arg(name));
    }
    _union = i.value();
}


Status UnionParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    static const QRegularExpression unionRe("^union +(\\w+)$");
    if (where == EOL)
    {
        return Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        if (line == "};")//{TODO:bug
        {
            insertCode(codeKey(BodyCodeKey),lines.mid(_start,_size),_union);
            reset();
            return Status::DoneWithRead;
        }
        else
        {
            _size++;
            return Status::Read;
        }
    case State::Declaration:
        if (line == "{")//}TODO:bug
        {
            _start = where+1;
            _size = 0;
            _state = State::Body;
        }
        return Status::Read;
    case State::Scanning:
    {
        auto match = unionRe.match(line);
        if (match.hasMatch())
        {
            find(match.captured(1));
            _state = State::Declaration;
            return Status::Read;
        }
        else
        {
            return Status::DoneWithoutRead;
        }
    }
    default:
        throw std::logic_error("unknown state");
    }
}


void UnionParser::populate(
)
{
    using LogicalParse = ::Exception::LogicalParse;
    for (int i = 0;i < block()->size();i++)
    {
        auto child = block()->get(i);
        if (child->meta()->index() == UnionIndex)
        {
            auto ub = qobject_cast<Union*>(child);
            if (_unions.contains(ub->name()))
            {
                throw LogicalParse(tr("Duplicate union name %1 encountered.").arg(ub->name()));
            }
            _unions.insert(ub->name(),ub);
        }
    }
}


void UnionParser::reset(
)
{
    _union = nullptr;
    _state = State::Scanning;
}
}
}
