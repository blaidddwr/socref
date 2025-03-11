#ifndef CPP_BLOCKWIDGET_UNIONEDIT_H
#define CPP_BLOCKWIDGET_UNIONEDIT_H
#include "CppBlockWidgetNamespaceEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of a union C++ block.
 */
class UnionEdit:
    public NamespaceEdit
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
