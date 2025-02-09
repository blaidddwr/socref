#include "ControllerCodeCppHeadParser.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "BlockCpp.h"
#include "ControllerCodeCpp.h"
#include "ModelMetaBlock.h"
namespace Controller {
namespace Code {
namespace Cpp {
using namespace Block::Cpp;
using Status = AbstractParser::Status;


HeadParser::HeadParser(
    AbstractParser* parent
):
    AbstractParser(parent)
{
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
        return Status::DoneWithoutRead;
    default:
        Q_ASSERT(false);
        std::exit(-1);
    }
}


void HeadParser::setBlock(
    Block::Abstract* object
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
    const static QRegularExpression guardRe("^#define\\s[A-Z_]+_H$");
    const static QRegularExpression namespaceRe("^namespace\\s[a-zA-Z_]\\w*\\s{$");//}TODO:bug
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        return Status::DelegateToChildren;
    case State::Header:
        if (
            line.isEmpty()
            || namespaceRe.match(line).hasMatch()
            )
        {
            block()->code().insert("headFile",_head);
            //addChild(new NamespaceParser(this,block(),version())); TODO
            //addChild(new ClassParser(this,block(),version())); TODO
            //addChild(new UnionParser(this,block(),version())); TODO
            _state = State::Body;
            return Status::DelegateToChildren;
        }
        else
        {
            _head.append(line);
            return Status::Read;
        }
    case State::Guard:
        if (guardRe.match(line).hasMatch())
        {
            _state = State::Header;
        }
        return Status::Read;
    default:
        Q_ASSERT(false);
        std::exit(-1);
    }
}


Status HeadParser::parseVersion1(
    const QStringList& lines
    ,int where
)
{
    const static QString headerBegin("/*@ header @*/");
    const static QString end("/*@ end @*/");
    const auto& line = lines.at(where);
    switch (_state)
    {
    case State::Body:
        return Status::DelegateToChildren;
    case State::Header:
        if (line == end)
        {
            block()->code().insert("headFile",_head);
            //addChild(new NamespaceParser(this,block(),version())); TODO
            //addChild(new ClassParser(this,block(),version())); TODO
            //addChild(new UnionParser(this,block(),version())); TODO
            _state = State::Body;
            return Status::Read;
        }
        else
        {
            _head.append(line);
            return Status::Read;
        }
    case State::Guard:
        if (line == headerBegin)
        {
            _state = State::Header;
        }
        return Status::Read;
    default:
        Q_ASSERT(false);
        std::exit(-1);
    }
}
}
}
}
