#ifndef WIDGET_BLOCK_CPP_UNION_H
#define WIDGET_BLOCK_CPP_UNION_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of a union C++ block.
 */
class Union:
    public Base
{
    Q_OBJECT


    public:
    Union(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
