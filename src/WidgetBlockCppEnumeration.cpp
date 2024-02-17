#include "WidgetBlockCppEnumeration.h"
#include <QtWidgets>
namespace Widget {
namespace Block {
namespace Cpp {


Enumeration::Enumeration(
    const ::Block::Abstract* block
    ,QWidget* parent
):
    Base(block,parent)
{
    setLayout(formLayout());
}
}
}
}
