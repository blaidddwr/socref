#include "WidgetBlockCppExceptionItem.h"
#include <QtWidgets>
namespace Widget {
namespace Block {
namespace Cpp {


ExceptionItem::ExceptionItem(
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
