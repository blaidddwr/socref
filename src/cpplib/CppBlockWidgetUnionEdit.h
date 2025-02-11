#ifndef CPP_BLOCKWIDGET_UNIONEDIT_H
#define CPP_BLOCKWIDGET_UNIONEDIT_H
#include "CppBlockWidgetBaseEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of a union C++ block.
 */
class UnionEdit:
    public BaseEdit
{
    Q_OBJECT


    public:
    UnionEdit(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
    );
};
}
}


#endif
