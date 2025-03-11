#ifndef TESTROUTER_H
#define TESTROUTER_H
#include "AbstractRouter.h"
#include "TestBlock.h"

class TestRouter : public AbstractRouter
{
    Q_OBJECT
public:
    QList<Shape::Route> _routes;
    QSet<QString> _names;
    int _collisions;
    using AbstractRouter::AbstractRouter;
    virtual QList<Shape::Route> routes(AbstractBlock* root) override final
    {
        _collisions = 0;
        _names.clear();
        _routes.clear();
        addRoutes(root,"");
        return _routes;
    }
    void addRoutes(AbstractBlock* block, const QString& scope)
    {
        auto b = qobject_cast<TestBlock*>(block);
        Q_ASSERT(b);
        auto name = scope+b->property1;
        if (_names.contains(name))
        {
            _collisions++;
        }
        else
        {
            _names.insert(name);
        }
        _routes.append({name,block,0,0});
        for (int i = 0;i < block->size();i++)
        {
            addRoutes(block->get(i),name);
        }
    }
};


#endif
