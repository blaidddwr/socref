#include "WidgetBlockCppNamespace.h"
#include <QtWidgets>
namespace Widget {
namespace Block {
namespace Cpp {


Namespace::Namespace(
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
