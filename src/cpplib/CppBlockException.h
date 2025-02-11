#ifndef CPP_BLOCK_EXCEPTION_H
#define CPP_BLOCK_EXCEPTION_H
#include "CppBlockBase.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ exception that can be thrown
 * from within a function.
 */
class Exception:
    public Base
{
    Q_OBJECT


    public:
    Exception(
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
