#include "CppParseClassParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockClass.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;
const QRegularExpression ClassParser::_classRe("^(template *<.*> +)?class +(\\w+)");


ClassParser::ClassParser(
    AbstractBlock* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
    Q_ASSERT(block);
    switch (block->meta()->index())
    {
    case ClassIndex:
    {
        auto cb = qobject_cast<Class*>(block);
        _classes.insert(cb->name(),cb);
        break;
    }
    case NamespaceIndex:
        populate();
        break;
    default:
        throw std::logic_error("invalid block");
    }
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


void ClassParser::find(
    const QString& name
)
{
    using LogicalParse = ::Exception::LogicalParse;
    auto i = _classes.find(name);
    if (i == _classes.end())
    {
        throw LogicalParse(tr("Encountered unknown class name %1.").arg(name));
    }
    _class = i.value();
}


Status ClassParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
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
                reset();
                return Status::DoneWithRead;
            }
            else
            {
                return Status::Read;
            }
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
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size),_class);
                _state = State::Body;
            }
            else if (line == "};")//{TODO:bug
            {
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size),_class);
                reset();
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
                find(match.captured(2));
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
            else if (line == "};")//{TODO:bug
            {
                reset();
                return Status::DoneWithRead;
            }
            else
            {
                return Status::Read;
            }
        case State::Declaration:
            if (line == "{")//}:TODO:bug
            {
                _start = where+1;
                _size = 0;
                _state = State::Header;
            }
            return Status::Read;
        case State::Footer:
            if (line == "};")//{TODO:bug
            {
                insertCode(codeKey(FooterCodeKey),lines.mid(_start,_size),_class);
                reset();
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
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size),_class);
                _start = where+1;
                _size = 0;
                _state = (line == footerLine) ? State::Footer : State::Body;
            }
            else if (line == "};")//{TODO:bug
            {
                insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size),_class);
                reset();
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
                find(match.captured(2));
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


void ClassParser::populate(
)
{
    using LogicalParse = ::Exception::LogicalParse;
    for (int i = 0;i < block()->size();i++)
    {
        auto child = block()->get(i);
        if (child->meta()->index() == ClassIndex)
        {
            auto cb = qobject_cast<Class*>(child);
            if (_classes.contains(cb->name()))
            {
                throw LogicalParse(tr("Duplicate class name %1 encountered.").arg(cb->name()));
            }
            _classes.insert(cb->name(),cb);
        }
    }
}


void ClassParser::reset(
)
{
    _class = nullptr;
    _state = State::Scanning;
}
}
}
