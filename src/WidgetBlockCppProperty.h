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
    public Widget::Block::Cpp::Base
{
    Q_OBJECT


    public:
    Property(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );


    /*!
     * Constructs this new widget with the given block, layout, and parent.
     * 
     * The given block and parent are passed to the parent constructor.
     * 
     * The given layout is set as this widget's layout. If it is null then this
     * widget will have no set layout.
     *
     * @param block
     *        The block.
     *
     * @param layout
     *        The layout.
     *
     * @param parent
     *        The parent.
     */
    protected:
    Property(
        const ::Block::Abstract* block
        ,QFormLayout* layout
        ,QWidget* parent = nullptr
    );
};
}
}
}


#endif
