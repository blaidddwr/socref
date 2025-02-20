#include "CppParseSourceParser.h"
#include <QtCore>
#include "AbstractBlock.h"
#include "Cpp.h"
#include "CppBlock.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;


SourceParser::SourceParser(
    AbstractParser* parent
):
    BaseParser(parent)
{
}


Status SourceParser::parse(
    const QStringList& lines
    ,int where
)
{
    using LogicalParse = ::Exception::LogicalParse;
    switch (version())
    {
    case Cpp_Legacy:
        return parseLegacy(lines,where);
    case Cpp_1:
        return Status::DoneWithoutRead;
    default:
        throw LogicalParse(tr("Unknown source code version %1.").arg(version()));
    }
}


void SourceParser::reset(
)
{
    _header.clear();
    _preProcess.clear();
    _state = State::Include;
}


void SourceParser::setBlock(
    AbstractBlock* object
)
{
    Q_ASSERT(
        object->meta()->index() == ClassIndex
        || object->meta()->index() == NamespaceIndex
        );
    AbstractParser::setBlock(object);
}


void SourceParser::setVersion(
    int value
)
{
    Q_ASSERT(value >= Cpp_Legacy);
    Q_ASSERT(value <= Cpp_Current);
    AbstractParser::setVersion(value);
}


Status SourceParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    const static QRegularExpression namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug
    auto line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        return line.isEmpty() ? Status::Read : Status::DelegateToChildren;
    case State::Header:
        if (line.isEmpty())
        {
            insertCode(HeaderSourceCodeKey,_header);
            //TODO: add function parser child
            _state = State::Body;
            return Status::Read;
        }
        else
        {
            _header.append(line);
            return Status::Read;
        }
    case State::Include:
        _state = State::PreProcess;
        return Status::Read;
    case State::Namespace:
        if (!namespaceRe.match(line).hasMatch())
        {
            if (line.isEmpty())
            {
                _state = State::Body;
            }
            else
            {
                _header.append(line);
                _state = State::Header;
            }
            return Status::Read;
        }
        else
        {
            return Status::Read;
        }
    case State::PreProcess:
        if (
            line.isEmpty()
            || namespaceRe.match(line).hasMatch()
            )
        {
            insertCode(PreProcessSourceCodeKey,_preProcess);
            _state = State::Namespace;
            return Status::Read;
        }
        else
        {
            _preProcess.append(line);
            return Status::Read;
        }
    default:
        throw std::logic_error("unknown state");
    }
}


Status SourceParser::parseVersion1(
    const QStringList& lines
    ,int where
)
{
    Q_UNUSED(lines);
    Q_UNUSED(where);
    //TODO
    return Status::DoneWithoutRead;
}
}
}
