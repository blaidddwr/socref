#ifndef CPP_BLOCK_EXCEPTION_H
#define CPP_BLOCK_EXCEPTION_H
#include "CppBlockNamespace.h"
namespace Cpp {
namespace Block {




/*!
 * This is a C++ block class. It represents a C++ exception that can be thrown
 * from within a function.
 */
class Exception:
    public Namespace
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
    virtual void addEvent(
        int index
    ) override final;


    protected:
    virtual AbstractBlock* create(
        QObject* parent = nullptr
    ) const override final;


    protected:
    virtual void removeEvent(
        int index
    ) override final;
};
}
}


#endif
