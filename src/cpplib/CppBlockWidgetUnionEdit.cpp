#include "CppBlockWidgetUnionEdit.h"
#include <QtWidgets>
namespace Cpp {
namespace BlockWidget {


UnionEdit::UnionEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    BaseEdit(block,parent)
{
    setLayout(formLayout());
}
}
}
