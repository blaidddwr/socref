#include "WidgetBlockCppProperty.h"
#include <QtWidgets>
namespace Widget {
namespace Block {
namespace Cpp {


Property::Property(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}


Property::Property(
    const ::Block::Abstract* block
    ,QFormLayout* layout
    ,QWidget* parent
):
    Base(block,parent)
{
    if (layout)
    {
        setLayout(layout);
    }
}
}
}
}
