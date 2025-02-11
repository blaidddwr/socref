#ifndef CPP_BLOCKWIDGET_NAMESPACEEDIT_H
#define CPP_BLOCKWIDGET_NAMESPACEEDIT_H
#include "CppBlockWidgetBaseEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of a namespace C++
 * block.
 */
class NamespaceEdit:
    public BaseEdit
{
    Q_OBJECT


    public:
    NamespaceEdit(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
    );
};
}
}


#endif
