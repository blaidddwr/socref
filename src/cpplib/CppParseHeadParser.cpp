#include "CppParseHeadParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockNamespace.h"
#include "CppParseClassParser.h"
#include "CppParseFunctionParser.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
#define END_NEWLINE_SIZE 2
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;
const QRegularExpression HeadParser::_endScopeRe("^}+$");//{TODO:bug
const QRegularExpression HeadParser::_guardRe("^#define [A-Z_]+_H$");
const QRegularExpression HeadParser::_namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug


HeadParser::HeadParser(
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
    new ClassParser(block,version,this);
    new FunctionParser(block,version,this);
}


Status HeadParser::parse(
    const QStringList& lines
    ,int where
)
{
    switch (version())
    {
    case Cpp_Legacy:
        return parseLegacy(lines,where);
    case Cpp_1:
        return parseVersion1(lines,where);
    default:
        throw std::logic_error("unknown version");
    }
}


Status HeadParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    if (where == EOL)
    {
        return _state == State::Body ? Status::DoneWithRead : Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        return line.isEmpty() ? Status::Read : Status::DelegateToChildren;
    case State::Guard:
        if (_guardRe.match(line).hasMatch())
        {
            _start = where+1;
            _size = 0;
            _state = State::PreProcess;
        }
        return Status::Read;
    case State::Namespace:
        if (line.isEmpty())
        {
            _state = State::Body;
        }
        return Status::Read;
    case State::PreProcess:
        if (
            line.isEmpty()
            || _namespaceRe.match(line).hasMatch()
            )
        {
            insertCode(codeKey(PreProcessHeadCodeKey),lines.mid(_start,_size));
            _state = line.isEmpty() ? State::Body : State::Namespace;
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


Status HeadParser::parseVersion1(
    const QStringList& lines
    ,int where
)
{
    static const QString doxygenLine = "/*!";
    static const QString footerLine = "//nsfooter:";
    if (where == EOL)
    {
        if (
            _state == State::End
            || _state == State::Body
            )
        {
            return Status::DoneWithRead;
        }
        else
        {
            return Status::DoneWithoutRead;
        }
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        if (line.isEmpty())
        {
            return Status::Read;
        }
        else if (line == doxygenLine)
        {
            _state = State::End;
            return Status::Read;
        }
        else if (line == footerLine)
        {
            _start = where+1;
            _size = 0;
            _state = State::Footer;
            return Status::Read;
        }
        else
        {
            return Status::DelegateToChildren;
        }
    case State::End:
        return Status::Read;
    case State::Footer:
        if (
            line.isEmpty()
            || _endScopeRe.match(line).hasMatch()
            )
        {
            insertCode(codeKey(FooterHeadCodeKey),lines.mid(_start,_size));
            _state = State::Body;
        }
        else
        {
            _size++;
        }
        return Status::Read;
    case State::Guard:
        if (_guardRe.match(line).hasMatch())
        {
            _start = where+1;
            _size = 0;
            _state = State::PreProcess;
        }
        return Status::Read;
    case State::Header:
        if (line.isEmpty())
        {
            insertCode(codeKey(HeaderHeadCodeKey),lines.mid(_start,_size));
            _empty = 1;
            _state = State::Body;
        }
        else
        {
            _size++;
        }
        return Status::Read;
    case State::Namespace:
        if (line.isEmpty())
        {
            _empty = 1;
            _state = State::Body;
        }
        else if (!_namespaceRe.match(line).hasMatch())
        {
            _start = where;
            _size = 1;
            _state = State::Header;
        }
        return Status::Read;
    case State::PreProcess:
        if (line.isEmpty())
        {
            insertCode(codeKey(PreProcessHeadCodeKey),lines.mid(_start,_size));
            _empty = 1;
            _state = State::Body;
        }
        else if (_namespaceRe.match(line).hasMatch())
        {
            insertCode(codeKey(PreProcessHeadCodeKey),lines.mid(_start,_size));
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
