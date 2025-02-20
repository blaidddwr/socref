#include "CppParseHeadParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlock.h"
#include "CppBlockNamespace.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;


HeadParser::HeadParser(
    AbstractParser* parent
):
    BaseParser(parent)
{
}


Status HeadParser::parse(
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


void HeadParser::reset(
)
{
    _preProcess.clear();
    _state = State::Guard;
}


void HeadParser::setBlock(
    AbstractBlock* object
)
{
    Q_ASSERT(
        object->meta()->index() == ClassIndex
        || object->meta()->index() == NamespaceIndex
        );
    AbstractParser::setBlock(object);
}


void HeadParser::setVersion(
    int value
)
{
    Q_ASSERT(value >= Cpp_Legacy);
    Q_ASSERT(value <= Cpp_Current);
    AbstractParser::setVersion(value);
}


Status HeadParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    const static QRegularExpression guardRe("^#define [A-Z_]+_H$");
    const static QRegularExpression namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        return line.isEmpty() ? Status::Read : Status::DelegateToChildren;
    case State::Guard:
        if (guardRe.match(line).hasMatch())
        {
            _state = State::PreProcess;
        }
        return Status::Read;
    case State::Namespace:
        if (line.isEmpty())
        {
            //TODO: add class parser child
            //TODO: add union parser child IF this parser's block is a namespace
            _state = State::Body;
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
            insertCode(PreProcessHeadCodeKey,_preProcess);
            _state = line.isEmpty() ? State::Body : State::Namespace;
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


Status HeadParser::parseVersion1(
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
