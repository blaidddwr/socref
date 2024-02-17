#ifndef WIDGET_BLOCK_CPP_ENUMERATION_H
#define WIDGET_BLOCK_CPP_ENUMERATION_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of an enumeration C++
 * block.
 */
class Enumeration:
    public Widget::Block::Cpp::Base
{
    Q_OBJECT


    public:
    Enumeration(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
