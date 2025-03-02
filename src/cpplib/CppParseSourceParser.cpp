#include "CppParseSourceParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockClass.h"
#include "CppBlockNamespace.h"
#include "CppParseFunctionParser.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;


SourceParser::SourceParser(
    AbstractBlock* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
    Q_ASSERT(block);
    Q_ASSERT(
        block->meta()->index() == ClassIndex
        || block->meta()->index() == NamespaceIndex
        );
    new FunctionParser(block,version,this);
}


Status SourceParser::parse(
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


Status SourceParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    const static QRegularExpression namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug
    if (where == EOL)
    {
        return _state == State::Body ? Status::DoneWithRead : Status::DoneWithoutRead;
    }
    auto line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        return line.isEmpty() ? Status::Read : Status::DelegateToChildren;
    case State::Header:
        if (line.isEmpty())
        {
            insertCode(codeKey(HeaderSourceCodeKey),lines.mid(_start,_size));
            _state = State::Body;
        }
        else
        {
            _size++;
        }
        return Status::Read;
    case State::Include:
        _start = 1;
        _size = 0;
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
                _start = where;
                _size = 1;
                _state = State::Header;
            }
            return Status::Read;
        }
        else
        {
            return Status::Read;
        }
    case State::PreProcess:
        if (line.isEmpty())
        {
            insertCode(codeKey(PreProcessSourceCodeKey),lines.mid(_start,_size));
            _state = State::Body;
        }
        else if (namespaceRe.match(line).hasMatch())
        {
            insertCode(codeKey(PreProcessSourceCodeKey),lines.mid(_start,_size));
            _state = State::Namespace;
        }
        else
        {
            _size++;
        }
        return Status::Read;
    default:
        throw std::logic_error("unknown state");
    }
}
}
}
