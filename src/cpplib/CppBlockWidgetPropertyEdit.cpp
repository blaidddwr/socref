#include "CppBlockWidgetPropertyEdit.h"
#include <QtWidgets>
namespace Cpp {
namespace BlockWidget {


PropertyEdit::PropertyEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    BaseEdit(block,parent)
{
    setLayout(formLayout());
}
}
}
