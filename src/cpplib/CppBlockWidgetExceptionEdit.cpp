#include "CppBlockWidgetExceptionEdit.h"
#include <QtWidgets>
namespace Cpp {
namespace BlockWidget {


ExceptionEdit::ExceptionEdit(
    const AbstractBlock* block
    ,QWidget* parent
):
    BaseEdit(block,parent)
{
    setLayout(formLayout());
}
}
}
