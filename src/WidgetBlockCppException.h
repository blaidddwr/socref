#ifndef WIDGET_BLOCK_CPP_EXCEPTION_H
#define WIDGET_BLOCK_CPP_EXCEPTION_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of an exception item C++
 * block.
 */
class Exception:
    public Base
{
    Q_OBJECT


    public:
    Exception(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
