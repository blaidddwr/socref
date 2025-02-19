#ifndef CPP_BLOCK_NAMESPACE_H
#define CPP_BLOCK_NAMESPACE_H
#include "CppBlockBase.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ namespace.
 */
class Namespace:
    public Base
{
    Q_OBJECT


    public:
    Namespace(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual AbstractBlockWidget* createWidget(
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;


    protected:
    virtual AbstractBlock* create(
        QObject* parent = nullptr
    ) const override final;
};
}
}


#endif
