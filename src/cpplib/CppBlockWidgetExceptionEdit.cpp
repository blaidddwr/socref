#include "CppBlockWidgetExceptionEdit.h"
#include <QtWidgets>
namespace Cpp {
namespace BlockWidget {


ExceptionEdit::ExceptionEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    NamespaceEdit(block,parent)
{
    setLayout(formLayout());
}
}
}
