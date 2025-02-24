#include "CppParseHeadParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockClass.h"
#include "CppBlockNamespace.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;


HeadParser::HeadParser(
    Class* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
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
    const static QRegularExpression guardRe("^#define [A-Z_]+_H$");
    const static QRegularExpression namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug
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
        if (guardRe.match(line).hasMatch())
        {
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
            || namespaceRe.match(line).hasMatch()
            )
        {
            insertCode(codeKey(PreProcessHeadCodeKey),_preProcess);
            _state = line.isEmpty() ? State::Body : State::Namespace;
        }
        else
        {
            _preProcess.append(line);
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
    const static QString endLine = "/*@ end @*/";
    const static QString endOfSourceLine = "/*@ EOS @*/";
    const static QString footerLine = "/*@ footer @*/";
    const static QString headerLine = "/*@ header @*/";
    const static QRegularExpression endScopeRe("^}+$");//{TODO:bug
    const static QRegularExpression guardRe("^#define [A-Z_]+_H$");
    const static QRegularExpression namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug
    if (where == EOL)
    {
        return _state == State::End ? Status::DoneWithRead : Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        if (line == footerLine)
        {
            _state = State::Footer;
            return Status::Read;
        }
        else if (line == endOfSourceLine)
        {
            _state = State::End;
            return Status::Read;
        }
        else if (line.isEmpty())
        {
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
            line == endLine
            || endScopeRe.match(line).hasMatch())
        {
            insertCode(codeKey(FooterHeadCodeKey),_footer);
            _state = State::End;
        }
        else
        {
            _footer.append(line);
        }
        return Status::Read;
    case State::Guard:
        if (guardRe.match(line).hasMatch())
        {
            _state = State::PreProcess;
        }
        return Status::Read;
    case State::Header:
        if (line.isEmpty())
        {
            insertCode(codeKey(HeaderHeadCodeKey),_header);
            _state = State::Body;
        }
        else
        {
            _header.append(line);
        }
        return Status::Read;
    case State::Namespace:
        if (line.isEmpty())
        {
            _state = State::Body;
        }
        else if (!namespaceRe.match(line).hasMatch())
        {
            _header.append(line);
            _state = State::Header;
        }
        return Status::Read;
    case State::PreProcess:
    {
        bool finished = false;
        if (line.isEmpty())
        {
            _state = State::Body;
            finished = true;
        }
        else if (line == headerLine)
        {
            _state = State::Header;
            finished = true;
        }
        else if (namespaceRe.match(line).hasMatch())
        {
            _state = State::Namespace;
            finished = true;
        }
        if (finished)
        {
            insertCode(codeKey(PreProcessHeadCodeKey),_preProcess);
        }
        else
        {
            _preProcess.append(line);
        }
        return Status::Read;
    }
    default:
        throw std::logic_error("unknown state");
    }
}
}
}
