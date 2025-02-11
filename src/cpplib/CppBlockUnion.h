#ifndef CPP_BLOCK_UNION_H
#define CPP_BLOCK_UNION_H
#include "CppBlockBase.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ union.
 */
class Union:
    public Base
{
    Q_OBJECT


    public:
    Union(
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
