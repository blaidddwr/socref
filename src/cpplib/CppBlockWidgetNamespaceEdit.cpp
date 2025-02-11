#include "CppBlockWidgetNamespaceEdit.h"
#include <QtWidgets>
namespace Cpp {
namespace BlockWidget {


NamespaceEdit::NamespaceEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    BaseEdit(block,parent)
{
    setLayout(formLayout());
}
}
}
