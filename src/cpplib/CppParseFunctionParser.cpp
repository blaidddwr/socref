#include "CppParseFunctionParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "Exception.h"
namespace Cpp {
namespace Parse {
using Status = AbstractParser::Status;
using Class = Block::Class;
using Namespace = Block::Namespace;
using Property = Block::Property;


FunctionParser::FunctionParser(
    AbstractBlock* block
    ,int version
    ,QObject* parent
):
    AbstractParser(block,version,parent)
{
    Q_ASSERT(version >= Cpp_Legacy);
    Q_ASSERT(version <= Cpp_Current);
    if (auto nb = qobject_cast<Namespace*>(block))
    {
        populate(nb,"");
    }
    else if (auto cb = qobject_cast<Class*>(block))
    {
        populate(cb,cb->name());
    }
    else
    {
        throw std::logic_error("invalid block");
    }
}


Status FunctionParser::parse(
    const QStringList& lines
    ,int where
)
{
    using LogicalParse = ::Exception::LogicalParse;
    switch (version())
    {
    case Cpp_Legacy:
    case Cpp_1:
        return parseLegacy(lines,where);
    default:
        throw LogicalParse(tr("Unknown source code version %1.").arg(version()));
    }
}


QStringList FunctionParser::detangle(
    const QString& arguments
)
{
    QStringList ret;
    const auto list = arguments.split(',',Qt::KeepEmptyParts);
    for (const auto& arg: list)
    {
        ret.append(toType(arg));
    }
    return ret;
}


void FunctionParser::find(
)
{
    using LogicalParse = ::Exception::LogicalParse;
    QString signature(_scope+_name+"("+_arguments.join(",")+")"+(_isConstant ? " const" : ""));
    auto i = _functions.find(signature);
    if (i == _functions.end())
    {
        throw LogicalParse(tr("Could not find parsed function %1 in project.").arg(signature));
    }
    _function = i.value();
}


Status FunctionParser::parseLegacy(
    const QStringList& lines
    ,int where
)
{
    static const QRegularExpression constantRe(" +const( +noexcept)?:?$");
    static const QRegularExpression endDeclarationRe("^ *)");
    static const QRegularExpression functionRe(
        "(= +)?((\\w+)::)?((\\w+)|(operator.*))\\((([^\\(\\n]*)\\))?( +const)?( +noexcept)?:?$"
        );
    if (where == EOL)
    {
        return Status::DoneWithoutRead;
    }
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        if (line == "}")//{TODO:bug
        {
            insertCode(codeKey(BodyCodeKey),lines.mid(_start,_size),_function);
            reset();
            return Status::DoneWithRead;
        }
        else
        {
            _size++;
            return Status::Read;
        }
    case State::Declaration:
        if (!line.isEmpty())
        {
            if (endDeclarationRe.match(line).hasMatch())
            {
                if (line.endsWith(';'))
                {
                    reset();
                    return Status::DoneWithRead;
                }
                _isConstant = constantRe.match(line).hasMatch();
                find();
                _start = where+1;
                _size = 0;
                _state = line.endsWith(':') ? State::Header : State::Middle;
            }
            else
            {
                _arguments.append(detangle(line));
            }
        }
        return Status::Read;
    case State::Header:
        if (line == "{")//}TODO:bug
        {
            insertCode(codeKey(HeaderCodeKey),lines.mid(_start,_size),_function);
            _start = where+1;
            _size = 0;
            _state = State::Body;
        }
        else
        {
            _size++;
        }
        return Status::Read;
    case State::Middle:
        if (line == "{")//}TODO:bug
        {
            _start = where+1;
            _size = 0;
            _state = State::Body;
        }
        return Status::Read;
    case State::Scanning:
        if (!line.startsWith(' '))
        {
            auto match = functionRe.match(line);
            if (
                match.hasMatch()
                && match.captured(1).isEmpty()
                )
            {
                _scope = match.captured(3).isEmpty() ? "" : match.captured(3)+"::";
                _name = match.captured(4);
                if (!match.captured(8).isEmpty())
                {
                    _arguments = detangle(match.captured(8));
                    _isConstant = !match.captured(9).isEmpty();
                    find();
                    _start = where+1;
                    _size = 0;
                    _state = line.endsWith(':') ? State::Header : State::Middle;
                }
                else
                {
                    _state = State::Declaration;
                }
                return Status::Read;
            }
        }
        return Status::DoneWithoutRead;
    default:
        throw std::logic_error("unkonwn state");
    }
}


void FunctionParser::populate(
    AbstractBlock* parent
    ,const QString& scope
)
{
    using LogicalParse = ::Exception::LogicalParse;
    for (auto child: parent->children())
    {
        if (auto fb = qobject_cast<Function*>(child))
        {
            QString signature(
                scope
                + fb->name()
                + "("
                + fb->arguments(true).join(",")
                + ")"
                + (fb->isConstant() ? " const" : "")
                );
            if (_functions.contains(signature))
            {
                throw LogicalParse(
                    tr("Duplicate function signature %1 encountered.").arg(signature)
                    );
            }
            _functions.insert(signature,fb);
        }
        else if (auto cb = qobject_cast<Class*>(child))
        {
            Q_ASSERT(scope.isEmpty());
            populate(parent,cb->name()+"::");
        }
        else if (auto pb = qobject_cast<Property*>(child))
        {
            populate(pb,scope);
        }
    }
}


void FunctionParser::reset(
)
{
    _function = nullptr;
    _arguments.clear();
    _isConstant = false;
    _state = State::Scanning;
}


QString FunctionParser::toType(
    const QString& argument
)
{
    using LogicalParse = ::Exception::LogicalParse;
    if (argument.isEmpty())
    {
        throw LogicalParse(tr("Parsed invalid function argument."));
    }
    auto ret = argument.simplified();
    auto i = ret.lastIndexOf(' ');
    if (i < 0)
    {
        throw LogicalParse(tr("Parsed invalid function argument."));
    }
    Q_ASSERT(i <= ret.size());
    return ret.first(i);
}
}
}
