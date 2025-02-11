#ifndef CPP_BLOCKWIDGET_PROPERTYEDIT_H
#define CPP_BLOCKWIDGET_PROPERTYEDIT_H
#include "CppBlockWidgetBaseEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of a property C++ block.
 */
class PropertyEdit:
    public BaseEdit
{
    Q_OBJECT


    public:
    PropertyEdit(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
    );
};
}
}


#endif
