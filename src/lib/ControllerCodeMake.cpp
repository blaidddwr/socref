#include "ControllerCodeMake.h"
#include <QtCore>
#include "BlockAbstract.h"
#include "Controller.h"
#include "ControllerCodeAbstractParser.h"
#include "ControllerCodeAbstractRouter.h"
#include "LanguageAbstract.h"
#include "ModelProject.h"
namespace Controller {
namespace Code {


Make::Make(
    Model::Project* project
    ,QObject* parent
):
    QObject(parent)
    ,_project(project)
{
    Q_ASSERT(project);
    connect(project,&QObject::destroyed,this,&Make::onProjectDestroyed);
    if (_project->_root)
    {
        auto language = project->language();
        Q_ASSERT(language);
        auto router = language->router();
        Q_ASSERT(router);
        _routes = router->routes(project->_root);
    }
}


void Make::clear(
)
{
    clear(_project->_root);
}


const QString& Make::error(
) const
{
    return _error;
}


bool Make::parse(
    int index
)
{
    const static QRegularExpression versionRe("^\\/\\*@\\sversion\\s([0-9]+)\\s@\\*\\/$");
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < size());
    const auto& route = _routes.at(index);
    QDir path(_project->absoluteCodePath());
    auto fpath = path.absoluteFilePath(route.path);
    if (!QFileInfo::exists(fpath))
    {
        return true;
    }
    QFile file(fpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        _error = tr("Failed opening file %1: %2.").arg(fpath,file.errorString());
        return false;
    }
    QTextStream stream(&file);
    auto data = stream.readAll();
    auto lines = data.split("\n",Qt::KeepEmptyParts);
    auto language = _project->language();
    Q_ASSERT(language);
    std::unique_ptr<Controller::Code::AbstractParser> parser(
        language->createParser(route.parseIndex)
        );
    Q_ASSERT(parser);
    parser->setBlock(route.block);
    if (!lines.isEmpty())
    {
        int version = Code_Legacy;
        auto match = versionRe.match(lines.first());
        bool ok;
        version = match.captured(1).toInt(&ok);
        Q_ASSERT(ok);
        parser->setVersion(version);
        parse(parser.get(),lines,1);
    }
    return true;
}


int Make::size(
) const
{
    return _routes.size();
}


void Make::onProjectDestroyed(
    QObject* object
)
{
    if (_project == object)
    {
        Q_ASSERT(false);
        _project = nullptr;
    }
}


void Make::onProjectModified(
    bool value
)
{
    if (value)
    {
        Q_ASSERT(false);
        _project = nullptr;
    }
}


void Make::clear(
    Block::Abstract* block
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


int Make::parse(
    Controller::Code::AbstractParser* parser
    ,const QStringList& lines
    ,int where
)
{
    using Status = Controller::Code::AbstractParser::Status;
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
            break;
        case Status::DoneWithRead:
            return where+1;
        case Status::DoneWithoutRead:
            return where;;
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
}
