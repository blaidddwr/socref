#include "WidgetBlockCppUnion.h"
#include <QtWidgets>
namespace Widget {
namespace Block {
namespace Cpp {


Union::Union(
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
