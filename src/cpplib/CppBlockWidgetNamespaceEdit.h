#ifndef CPP_BLOCKWIDGET_NAMESPACEEDIT_H
#define CPP_BLOCKWIDGET_NAMESPACEEDIT_H
#include "AbstractBlockWidget.h"
class QFormLayout;
class QLineEdit;
class QTextEdit;
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of a C++ namespace
 * block.
 * 
 * It can be extended for other blocks by using the protected getter methods to
 * expand upon its GUI.
 */
class NamespaceEdit:
    public AbstractBlockWidget
{
    Q_OBJECT
    QFormLayout* _formLayout {nullptr};
    QLineEdit* _nameEdit {nullptr};
    QTextEdit* _descriptionEdit {nullptr};


    public:
    NamespaceEdit(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
    );


    public slots:
    virtual bool apply(
    ) override;


    /*!
     * Getter method.
     *
     * @return
     * This widget's description text edit widget.
     */
    protected:
    QTextEdit* descriptionTextEdit(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's form layout.
     */
    protected:
    QFormLayout* formLayout(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's name line edit widget.
     */
    protected:
    QLineEdit* nameLineEdit(
    );
};
}
}


#endif
