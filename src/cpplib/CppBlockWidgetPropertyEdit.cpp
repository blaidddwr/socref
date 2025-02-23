#include "CppBlockWidgetPropertyEdit.h"
#include <QtWidgets>
namespace Cpp {
namespace BlockWidget {


PropertyEdit::PropertyEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    NamespaceEdit(block,parent)
{
    setLayout(formLayout());
}
}
}
