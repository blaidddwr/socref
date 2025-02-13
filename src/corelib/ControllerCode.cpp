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
    QDir path(_project->absoluteCodePath());
    auto fpath = path.absoluteFilePath(route.path);
    if (!QFileInfo::exists(fpath))
    {
        return;
    }
    QFile file(fpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileSystem(
            tr("Failed opening source code file %1: %2.").arg(fpath,file.errorString())
            );
    }
    QTextStream stream(&file);
    auto data = stream.readAll();
    auto lines = data.split("\n",Qt::KeepEmptyParts);
    auto language = _project->language();
    Q_ASSERT(language);
    std::unique_ptr<AbstractParser> parser(language->createParser(route.parseIndex));
    Q_ASSERT(parser);
    parser->setBlock(route.block);
    if (!lines.isEmpty())
    {
        int version = CODE_LEGACY;
        auto match = versionRe.match(lines.first());
        bool ok;
        version = match.captured(1).toInt(&ok);
        Q_ASSERT(ok);
        parser->setVersion(version);
        parse(parser.get(),lines,1);
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
    ,const QStringList& lines
    ,int where
)
{
    using Status = AbstractParser::Status;
    Q_ASSERT(parser);
    while (where < lines.size())
    {
        switch (parser->parse(lines,where))
        {
        case Status::DelegateToChildren:
            for (auto child: parser->children())
            {
                int nw = parse(child,lines,where);
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
            Q_ASSERT(false);
            std::exit(-1);
            break;
        }
    }
    return where;
}
}
