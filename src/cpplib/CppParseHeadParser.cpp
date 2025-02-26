#include "CppParseHeadParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockClass.h"
#include "CppBlockNamespace.h"
#include "CppParseClassParser.h"
#include "Exception.h"
#define END_NEWLINE_SIZE 2
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;
const QRegularExpression HeadParser::_endScopeRe("^}+$");//{TODO:bug
const QRegularExpression HeadParser::_guardRe("^#define [A-Z_]+_H$");
const QRegularExpression HeadParser::_namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug


HeadParser::HeadParser(
    Class* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
    new ClassParser(block,version,this);
}


HeadParser::HeadParser(
    Namespace* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
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
        return parseVersion1(lines,where);
    default:
        throw LogicalParse(tr("Unknown source code version %1.").arg(version()));
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
    auto isFooter = [this,&lines,where]() -> bool
    {
        if (_endScopeRe.match(lines.at(where)).hasMatch())
        {
            return false;
        }
        auto i = where;
        while (
            i < lines.size()
            && !lines.at(i).isEmpty()
            )
        {
            i++;
        }
        int empty = 0;
        while (
            i < lines.size()
            && lines.at(i).isEmpty()
            )
        {
            empty++;
            i++;
            if (empty == END_NEWLINE_SIZE)
            {
                return true;
            }
        }
        return false;
    };
    if (where == EOL)
    {
        return _state == State::End ? Status::DoneWithRead : Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        if (line.isEmpty())
        {
            _empty++;
            if (_empty == END_NEWLINE_SIZE)
            {
                _state = State::End;
            }
            return Status::Read;
        }
        else if (_endScopeRe.match(line).hasMatch())
        {
            _state = State::End;
            return Status::Read;
        }
        else if (isFooter())
        {
            _start = where;
            _size = 1;
            _state = State::Footer;
            return Status::Read;
        }
        else
        {
            _empty = 0;
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
            _state = State::End;
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
