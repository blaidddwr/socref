#ifndef WIDGET_BLOCK_CPP_EXCEPTIONITEM_H
#define WIDGET_BLOCK_CPP_EXCEPTIONITEM_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of an exception item C++
 * block.
 */
class ExceptionItem:
    public Base
{
    Q_OBJECT


    public:
    ExceptionItem(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
