#include "ControllerCode.h"
#include <QtCore>
#include "AbstractBlock.h"
#include "AbstractParser.h"
#include "AbstractLanguage.h"
#include "AbstractRouter.h"
#include "Exception.h"
#include "Global.h"
#include "ModelProject.h"
namespace Controller {


Code::Code(
    Model::Project* project
    ,QObject* parent
):
    QObject(parent)
    ,_project(project)
{
    Q_ASSERT(project);
    connect(project,&QObject::destroyed,this,&Code::onProjectDestroyed);
    if (_project->_root)
    {
        auto language = project->language();
        Q_ASSERT(language);
        auto router = language->router();
        Q_ASSERT(router);
        _routes = router->routes(project->_root);
    }
}


void Code::clear(
)
{
    clear(_project->_root);
}


void Code::parse(
    int index
)
{
    using FileSystem = Exception::FileSystem;
    const static QRegularExpression versionRe("^\\/\\*@ version ([0-9]+) @\\*\\/$");
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < size());
    const auto& route = _routes.at(index);
    _path = QDir(_project->absoluteCodePath()).absoluteFilePath(route.path);
    if (!QFileInfo::exists(_path))
    {
        return;
    }
    QFile file(_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileSystem(
            tr("Failed opening source code file %1: %2.").arg(_path,file.errorString())
            );
    }
    QTextStream stream(&file);
    auto data = stream.readAll();
    _lines = data.split("\n",Qt::KeepEmptyParts);
    auto language = _project->language();
    Q_ASSERT(language);
    std::unique_ptr<AbstractParser> parser(language->createParser(route.parseIndex));
    Q_ASSERT(parser);
    parser->setBlock(route.block);
    if (!_lines.isEmpty())
    {
        int where = 0;
        int version = CODE_LEGACY;
        auto match = versionRe.match(_lines.first());
        if (match.hasMatch())
        {
            bool ok;
            version = match.captured(1).toInt(&ok);
            Q_ASSERT(ok);
            where++;
        }
        parser->setVersion(version);
        parse(parser.get(),where);
    }
}


int Code::size(
) const
{
    return _routes.size();
}


void Code::onProjectDestroyed(
    QObject* object
)
{
    if (_project == object)
    {
        Q_ASSERT(false);
        _project = nullptr;
    }
}


void Code::onProjectModified(
    bool value
)
{
    if (value)
    {
        Q_ASSERT(false);
        _project = nullptr;
    }
}


void Code::clear(
    AbstractBlock* block
)
{
    if (block)
    {
        block->code().clear();
        block->touchCode();
        for (int i = 0;i < block->size();i++)
        {
            clear(block->get(i));
        }
    }
}


int Code::parse(
    AbstractParser* parser
    ,int where
)
{
    using LogicalParse = Exception::LogicalParse;
    using Status = AbstractParser::Status;
    Q_ASSERT(parser);
    while (where < _lines.size())
    {
        switch (parser->parse(_lines,where))
        {
        case Status::DelegateToChildren:
            for (auto child: parser->children())
            {
                int nw = parse(child,where);
                if (nw != where)
                {
                    break;
                }
            }
            where++;
            break;
        case Status::DoneWithRead:
            return where+1;
        case Status::DoneWithoutRead:
            return where;
        case Status::Read:
            where++;
            break;
        default:
            throw std::logic_error("unknown parser status");
        }
    }
    if (parser->parse(_lines,AbstractParser::EOL) != Status::DoneWithRead)
    {
        throw LogicalParse(
            tr("Parser unexpectedly reached end of source code file %1.").arg(_path)
            );
    }
    return where;
}
}
