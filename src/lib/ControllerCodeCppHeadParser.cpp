#include "ControllerCodeCppHeadParser.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "BlockCpp.h"
#include "BlockCppNamespace.h"
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
    enum State
    {
        Body
        ,Header
        ,Namespaces
        ,PreProcess
        ,Guard
    };
    const static QRegularExpression guardRe("^#define [A-Z_]+_H$");
    const static QRegularExpression namespaceRe("^namespace [a-zA-Z_]\\w* {$");//}TODO:bug
    const auto& line = lines.at(where);
    switch (_state)
    {
    case Body:
        return Status::DelegateToChildren;
    case Header:
        if (line.isEmpty())
        {
            block()->code().insert(Namespace::HEADER_IN_HEADER_CODE_KEY,_header);
            //TODO: add class child IF AND ONLY IF this parser's block is a class
            _state = Body;
            return Status::Read;
        }
        else
        {
            _header.append(line);
            return Status::Read;
        }
    case Namespaces:
        if (!namespaceRe.match(line).hasMatch())
        {
            if (!line.isEmpty())
            {
                _header.append(line);
            }
            _state = Header;
        }
        return Status::Read;
    case PreProcess:
        if (namespaceRe.match(line).hasMatch())
        {
            block()->code().insert(Namespace::PREPROCESS_IN_HEADER_CODE_KEY,_preProcess);
            _state = Namespaces;
            return Status::Read;
        }
        else
        {
            _preProcess.append(line);
            return Status::Read;
        }
    case Guard:
        if (guardRe.match(line).hasMatch())
        {
            _state = PreProcess;
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
    Q_UNUSED(lines);
    Q_UNUSED(where);
    //TODO
    return Status::DoneWithoutRead;
}
}
}
}
