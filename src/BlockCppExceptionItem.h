#ifndef BLOCK_CPP_EXCEPTIONITEM_H
#define BLOCK_CPP_EXCEPTIONITEM_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ exception that can be thrown
 * from within a function.
 */
class ExceptionItem:
    public Block::Cpp::Base
{
    Q_OBJECT


    public:
    ExceptionItem(
        Model::Meta::Block* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual Widget::Block::Abstract* createWidget(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual QIcon displayIcon(
    ) const override final;
};
}
}


#endif
