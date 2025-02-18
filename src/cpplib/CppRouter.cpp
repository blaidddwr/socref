#include "CppRouter.h"
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "CppBlockNamespace.h"
#include "CppParse.h"
#include "Exception.h"
#include "ModelMetaBlock.h"
namespace Cpp {
using namespace Block;
using namespace Parse;


Router::Router(
    QObject* parent
):
    AbstractRouter(parent)
{
}


QList<Shape::Route> Router::routes(
    AbstractBlock* root
)
{
    Q_ASSERT(root);
    _scopes.clear();
    _routes.clear();
    addRoutes(root,"");
    return _routes;
}


void Router::addRoutes(
    AbstractBlock* block
    ,const QString& scope
)
{
    using LogicalRoute = ::Exception::LogicalRoute;
    switch (block->meta()->index())
    {
    case ClassIndex:
    {
        auto cb = qobject_cast<Class*>(block);
        Q_ASSERT(cb);
        auto newScope = scope+cb->name();
        if (_scopes.contains(newScope))
        {
            throw LogicalRoute(
                tr("Duplicate scope %1 in project causing collision of source code file names.")
                    .arg(newScope)
                );
        }
        _scopes.insert(newScope);
        _routes.append({newScope+".h",block,HeadParserIndex,-1});//TODO
        if (hasSource(cb))
        {
            _routes.append({newScope+".cpp",block,-1,-1});//TODO
        }
        break;
    }
    case NamespaceIndex:
    {
        auto nb = qobject_cast<Namespace*>(block);
        Q_ASSERT(nb);
        auto newScope = scope+nb->name();
        QString rootName = qobject_cast<AbstractBlock*>(nb->parent()) ? "" : "Global";
        if (_scopes.contains(newScope))
        {
            throw LogicalRoute(
                tr("Duplicate scope %1 in project causing collision of source code file names.")
                    .arg(newScope)
                );
        }
        _scopes.insert(newScope);
        if (hasHeader(nb))
        {
            _routes.append({newScope+rootName+".h",block,HeadParserIndex,-1});//TODO
        }
        if (hasSource(nb))
        {
            _routes.append({newScope+rootName+".cpp",block,-1,-1});//TODO
        }
        for (int i = 0;i < block->size();i++)
        {
            addRoutes(block->get(i),scope);
        }
        break;
    }
    }
}


bool Router::hasHeader(
    Namespace* block
)
{
    Q_ASSERT(block);
    for (int i = 0;i < block->size();i++)
    {
        auto child = block->get(i);
        switch (child->meta()->index())
        {
        case NamespaceIndex:
            break;
        default:
            return true;
        }
    }
    return false;
}


bool Router::hasSource(
    AbstractBlock* block
)
{
    Q_ASSERT(block);
    for (int i = 0;i < block->size();i++)
    {
        if (auto fb = qobject_cast<Function*>(block->get(i)))
        {
            if (fb->templates().isEmpty())
            {
                return true;
            }
        }
    }
    return false;
}
}
