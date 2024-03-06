#ifndef WIDGET_BLOCK_CPP_BASE_H
#define WIDGET_BLOCK_CPP_BASE_H
#include "WidgetBlockAbstract.h"
class QFormLayout;
class QLineEdit;
class QTextEdit;
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of base C++ blocks.
 * 
 * It is designed to be inherited and used by actual C++ block widget
 * implementations. The GUI can be created by using this class's protected
 * getter methods for individual widget elements. This class's form layout has
 * the name and description edit widgets added to it.
 */
class Base:
    public Abstract
{
    Q_OBJECT
    QFormLayout* _formLayout {nullptr};
    QLineEdit* _nameEdit {nullptr};
    QTextEdit* _descriptionEdit {nullptr};
    public:
    using Abstract::Abstract;


    public slots:
    virtual void apply(
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
}


#endif
