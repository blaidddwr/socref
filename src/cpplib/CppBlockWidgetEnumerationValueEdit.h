#ifndef CPP_BLOCKWIDGET_ENUMERATIONVALUEEDIT_H
#define CPP_BLOCKWIDGET_ENUMERATIONVALUEEDIT_H
#include "CppBlockWidgetNamespaceEdit.h"
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of an enumeration value
 * C++ block.
 */
class EnumerationValueEdit:
    public NamespaceEdit
{
    Q_OBJECT
    QLineEdit* _assignmentLineEdit {nullptr};
    bool _formLayout {false};


    public:
    EnumerationValueEdit(
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
};
}
}


#endif
