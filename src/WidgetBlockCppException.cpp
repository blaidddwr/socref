#include "WidgetBlockCppException.h"
#include <QtWidgets>
namespace Widget {
namespace Block {
namespace Cpp {


Exception::Exception(
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
