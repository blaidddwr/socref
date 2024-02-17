#ifndef WIDGET_BLOCK_CPP_NAMESPACE_H
#define WIDGET_BLOCK_CPP_NAMESPACE_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of a namespace C++
 * block.
 */
class Namespace:
    public Widget::Block::Cpp::Base
{
    Q_OBJECT


    public:
    Namespace(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
