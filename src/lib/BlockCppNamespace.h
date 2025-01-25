#ifndef BLOCK_CPP_NAMESPACE_H
#define BLOCK_CPP_NAMESPACE_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




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
    virtual Widget::Block::Abstract* createWidget(
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;


    protected:
    virtual Abstract* create(
        QObject* parent = nullptr
    ) const override final;
};
}
}


#endif
