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
    static const char* PREPROCESS_IN_HEADER_CODE_KEY;
    static const char* HEADER_IN_HEADER_CODE_KEY;


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
