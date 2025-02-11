#ifndef CPP_BLOCKWIDGET_BASEEDIT_H
#define CPP_BLOCKWIDGET_BASEEDIT_H
#include "AbstractBlockWidget.h"
class QFormLayout;
class QLineEdit;
class QTextEdit;
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of base C++ blocks.
 * 
 * It is designed to be inherited and used by actual C++ block widget
 * implementations. The GUI can be created by using this class's protected
 * getter methods for individual widget elements. This class's form layout has
 * the name and description edit widgets added to it.
 */
class BaseEdit:
    public AbstractBlockWidget
{
    Q_OBJECT
    QFormLayout* _formLayout {nullptr};
    QLineEdit* _nameEdit {nullptr};
    QTextEdit* _descriptionEdit {nullptr};


    public:
    BaseEdit(
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
