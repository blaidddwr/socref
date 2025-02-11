#ifndef CPP_BLOCKWIDGET_EXCEPTIONEDIT_H
#define CPP_BLOCKWIDGET_EXCEPTIONEDIT_H
#include "CppBlockWidgetBaseEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of an exception item C++
 * block.
 */
class ExceptionEdit:
    public BaseEdit
{
    Q_OBJECT


    public:
    ExceptionEdit(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
    );
};
}
}


#endif
