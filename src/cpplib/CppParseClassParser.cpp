#include "CppParseClassParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockClass.h"
#include "Exception.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;
const QRegularExpression ClassParser::_classRe("^class ([A-Za-z_]\\w*)");


ClassParser::ClassParser(
    Class* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(block);
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
}


Status ClassParser::parse(
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


Status ClassParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    using LogicalParse = ::Exception::LogicalParse;
    if (where == EOL)
    {
        return Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
        case State::Body:
            return (line == "};") ? Status::DoneWithRead : Status::Read;//{TODO:bug
        case State::Declaration:
            if (line == "{")//}:TODO:bug
            {
                _start = where+1;
                _size = 0;
                _state = State::Header;
            }
            return Status::Read;
        case State::Header:
            if (line.isEmpty())
            {
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size));
                _state = State::Body;
            }
            else if (line == "};")//{TODO:bug
            {
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size));
                return Status::DoneWithRead;
            }
            else
            {
                _size++;
            }
            return Status::Read;
        case State::Scanning:
        {
            auto match = _classRe.match(line);
            if (match.hasMatch())
            {
                auto name = match.captured(1);
                if (name != qobject_cast<Class*>(block())->name())
                {
                    throw LogicalParse(tr("Encountered unknown class name %1.").arg(name));
                }
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


Status ClassParser::parseVersion1(
    const QStringList& lines
    ,int where
)
{
    static const QString footerLine = "//footer:";
    using LogicalParse = ::Exception::LogicalParse;
    if (where == EOL)
    {
        return Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
        case State::Body:
            if (line == footerLine)
            {
                _start = where+1;
                _size = 0;
                _state = State::Footer;
                return Status::Read;
            }
            else
            {
                return (line == "};") ? Status::DoneWithRead : Status::Read; //{TODO:bug
            }
        case State::Declaration:
            if (line == "{")//}:TODO:bug
            {
                _start = where+1;
                _state = State::Header;
            }
            return Status::Read;
        case State::Footer:
            if (line == "};")//{TODO:bug
            {
                insertCode(codeKey(FooterCodeKey),lines.mid(_start,_size));
                return Status::DoneWithRead;
            }
            else
            {
                _size++;
                return Status::Read;
            }
        case State::Header:
            if (line.startsWith("//"))
            {
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size));
                _start = where+1;
                _size = 0;
                _state = (line == footerLine) ? State::Footer : State::Body;
            }
            else if (line == "};")//{TODO:bug
            {
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size));
                return Status::DoneWithRead;
            }
            else
            {
                _size++;
            }
            return Status::Read;
        case State::Scanning:
        {
            auto match = _classRe.match(line);
            if (match.hasMatch())
            {
                auto name = match.captured(1);
                if (name != qobject_cast<Class*>(block())->name())
                {
                    throw LogicalParse(tr("Encountered unknown class name %1.").arg(name));
                }
                _start = where+1;
                _size = 0;
                _state = line.endsWith("}") ? State::Header : State::Declaration;//{TODO:bug
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
}
}
