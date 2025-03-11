#ifndef CPP_ROUTER_H
#define CPP_ROUTER_H
#include <QSet>
#include "AbstractRouter.h"
#include "CppBlock.h"
namespace Cpp {




/*!
 * This is the C++ router.
 */
class Router:
    public AbstractRouter
{
    Q_OBJECT
    using Class = Block::Class;
    using Function = Block::Function;
    using Namespace = Block::Namespace;
    QSet<QString> _scopes;
    QList<Shape::Route> _routes;


    public:
    Router(
        QObject* parent = nullptr
    );


    public:
    virtual QList<Shape::Route> routes(
        AbstractBlock* root
    ) override final;


    /*!
     * Recursively adds routes to this method's instance.
     * 
     * @exception Exception::LogicalRoute Thrown when a logical route error is
     * encountered.
     *
     * @param block
     *        The block whose routes and all its descendant block's routes are
     *        added.
     *
     * @param scope
     *        The given block's scope, used to generate its own scope. This is
     *        used to generate the header and source code file names.
     */
    private:
    void addRoutes(
        AbstractBlock* block
        ,const QString& scope
    );


    /*!
     * Evaluates a block.
     *
     * @param block
     *        The evaluated block. This must be a valid block.
     *
     * @return
     * True if the evaluated block has a header file or false otherwise.
     */
    private:
    static bool hasHeader(
        Namespace* block
    );


    /*!
     * Evaluates a block.
     *
     * @param block
     *        The evaluated block. This must be a valid block.
     *
     * @return
     * True if the evaluated block has a source file or false otherwise.
     */
    private:
    static bool hasSource(
        AbstractBlock* block
    );
};
}


#endif
