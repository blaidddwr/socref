#include "CppParseFunctionParser.h"
#include <QtCore>
#include "Cpp.h"
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
namespace Parse {
using namespace Block;
using Status = AbstractParser::Status;


FunctionParser::FunctionParser(
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
        populate(block,qobject_cast<Class*>(block)->name()+"::");
        break;
    case NamespaceIndex:
        populate(block,"");
        break;
    default:
        throw std::logic_error("invalid block");
    }
}


Status FunctionParser::parse(
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


QStringList FunctionParser::detangle(
    const QString& arguments
)
{
    using LogicalParse = ::Exception::LogicalParse;
    QStringList ret;
    if (arguments.isEmpty())
    {
        return ret;
    }
    QString argument;
    int depth = 0;
    for (const auto& ch: arguments)
    {
        switch (ch.unicode())
        {
        case '<':
            depth++;
            break;
        case '>':
            depth--;
            break;
        case ',':
            if (depth == 0)
            {
                if (argument.isEmpty())
                {
                    throw LogicalParse(tr("Invalid function arguments '%1'.").arg(arguments));
                }
                ret.append(toType(argument));
                argument.clear();
            }
            break;
        default:
            argument.append(ch);
            break;
        }
        if (depth < 0)
        {
            throw LogicalParse(tr("Invalid function arguments '%1'.").arg(arguments));
        }
    }
    if (depth != 0)
    {
        throw LogicalParse(tr("Invalid function arguments '%1'.").arg(arguments));
    }
    if (argument.isEmpty())
    {
        throw LogicalParse(tr("Invalid function arguments '%1'.").arg(arguments));
    }
    ret.append(toType(argument));
    return ret;
}


void FunctionParser::find(
)
{
    using LogicalParse = ::Exception::LogicalParse;
    QString signature(_scope+_name+"("+_arguments.join(",")+")"+(_isConstant ? "const" : ""));
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
    static const QRegularExpression commaArgumentRe("^ *,?(.*)$");
    static const QRegularExpression constantRe(" +const( +noexcept)?:?$");
    static const QRegularExpression endDeclarationRe("^ *\\)");
    static const QRegularExpression functionRe(
        "(= +)?((\\w+)::)?"
        "((~?[A-Za-z_]\\w*)|(operator.*))"
        "\\((([^\\(]*)\\)( +const)?( +noexcept)?:?)?$"
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
                _arguments.append(toType(commaArgumentRe.match(line).captured(1)));
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
                if (!match.captured(7).isEmpty())
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
    for (int i = 0;i < parent->size();i++)
    {
        auto child = parent->get(i);
        switch (child->meta()->index())
        {
        case FunctionIndex:
        {
            auto fb = qobject_cast<Function*>(child);
            auto signature = scope+fb->signature();
            if (_functions.contains(signature))
            {
                throw LogicalParse(
                    tr("Duplicate function signature %1 encountered.").arg(signature)
                    );
            }
            _functions.insert(signature,fb);
            break;
        }
        case ClassIndex:
            Q_ASSERT(scope.isEmpty());
            populate(child,qobject_cast<Class*>(child)->name()+"::");
            break;
        case PropertyIndex:
            populate(child,scope);
            break;
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
    static const QRegularExpression argumentRe("^(const )?\\w+[\\w<>&*,]*$");
    using LogicalParse = ::Exception::LogicalParse;
    Q_ASSERT(!argument.isEmpty());
    auto ret = argument.simplified();
    auto i = ret.lastIndexOf(' ');
    if (i < 0)
    {
        throw LogicalParse(tr("Invalid function argument '%1'.").arg(argument));
    }
    Q_ASSERT(i <= ret.size());
    ret.resize(i);
    if (!argumentRe.match(ret).hasMatch())
    {
        throw LogicalParse(tr("Invalid function argument '%1'.").arg(argument));
    }
    return ret;
}
}
}
