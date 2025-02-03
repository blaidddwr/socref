#ifndef WIDGET_BLOCK_CPP_ENUMERATIONVALUE_H
#define WIDGET_BLOCK_CPP_ENUMERATIONVALUE_H
#include "WidgetBlockCppBase.h"
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of an enumeration value
 * C++ block.
 */
class EnumerationValue:
    public Base
{
    Q_OBJECT
    QLineEdit* _assignmentLineEdit {nullptr};
    bool _formLayout {false};


    public:
    EnumerationValue(
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
}


#endif
