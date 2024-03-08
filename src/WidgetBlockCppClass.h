#ifndef WIDGET_BLOCK_CPP_CLASS_H
#define WIDGET_BLOCK_CPP_CLASS_H
#include "Widget.h"
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget class. It provides an editable view of a class C++
 * block.
 */
class Class:
    public Base
{
    Q_OBJECT
    StringList* _parentList {nullptr};
    StringList* _templateList {nullptr};
    bool _formLayout {false};


    public:
    Class(
        const ::Block::Abstract* block
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
    StringList* parentStringList(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's template string list widget.
     */
    private:
    StringList* templateStringList(
    );
};
}
}
}


#endif
