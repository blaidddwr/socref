#ifndef WIDGET_BLOCK_CPP_PROPERTY_H
#define WIDGET_BLOCK_CPP_PROPERTY_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of a property C++ block.
 */
class Property:
    public Base
{
    Q_OBJECT


    public:
    Property(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
