#ifndef BLOCK_CPP_UNION_H
#define BLOCK_CPP_UNION_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ union.
 */
class Union:
    public Block::Cpp::Base
{
    Q_OBJECT


    public:
    Union(
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
    virtual Block::Abstract* create(
        QObject* parent = nullptr
    ) const override final;
};
}
}


#endif
