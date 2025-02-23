#ifndef CPP_BLOCKWIDGET_VARIABLEEDIT_H
#define CPP_BLOCKWIDGET_VARIABLEEDIT_H
#include "CppBlockWidgetNamespaceEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of a variable C++ block.
 */
class VariableEdit:
    public NamespaceEdit
{
    Q_OBJECT
    QLineEdit* _assignmentLineEdit {nullptr};
    QLineEdit* _typeLineEdit {nullptr};
    bool _formLayout {false};


    public:
    VariableEdit(
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


#endif
