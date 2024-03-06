#ifndef WIDGET_BLOCK_CPP_VARIABLE_H
#define WIDGET_BLOCK_CPP_VARIABLE_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of a variable C++ block.
 */
class Variable:
    public Base
{
    Q_OBJECT
    QLineEdit* _assignmentLineEdit {nullptr};
    QLineEdit* _typeLineEdit {nullptr};
    bool _formLayout {false};


    public:
    Variable(
        const ::Block::Abstract* block
        ,QWidget* parent = nullptr
    );


    public slots:
    virtual void apply(
    ) override final;


    /*!
     * Getter method.
     *
     * @return
     * This widget's assignment line edit widget.
     */
    private:
    QLineEdit* assignmentLineEdit(
    );


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
     * This widget's type line edit widget.
     */
    private:
    QLineEdit* typeLineEdit(
    );
};
}
}
}


#endif
