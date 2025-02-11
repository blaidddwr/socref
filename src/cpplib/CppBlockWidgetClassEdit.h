#ifndef CPP_BLOCKWIDGET_CLASSEDIT_H
#define CPP_BLOCKWIDGET_CLASSEDIT_H
#include "CppBlockWidgetBaseEdit.h"
#include "Widget.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget class. It provides an editable view of a class C++
 * block.
 */
class ClassEdit:
    public BaseEdit
{
    Q_OBJECT
    Widget::StringList* _parentList {nullptr};
    Widget::StringList* _templateList {nullptr};
    bool _formLayout {false};


    public:
    ClassEdit(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
    );


    public slots:
    virtual bool apply(
    ) override final;


    /*!
     * Getter method.
     *
     * @return
     * This widget's form layout.
     */
    private:
    QFormLayout* formLayout(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's parent string list widget.
     */
    private:
    Widget::StringList* parentStringList(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's template string list widget.
     */
    private:
    Widget::StringList* templateStringList(
    );
};
}
}


#endif
