#ifndef BLOCK_CPP_PROPERTY_H
#define BLOCK_CPP_PROPERTY_H
#include "BlockCppBase.h"
namespace Block {
namespace Cpp {




/*!
 * This is a C++ block class. It represents a C++ class property. While this is
 * not a concrete type defined in the C++ language, it is an abstract concept
 * used pervasively enough in classes to conceptualize it as a block. A
 * property's setter and getter are added as children to a property.
 */
class Property:
    public Block::Cpp::Base
{
    Q_OBJECT


    public:
    Property(
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
