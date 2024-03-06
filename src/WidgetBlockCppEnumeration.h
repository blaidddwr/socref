#ifndef WIDGET_BLOCK_CPP_ENUMERATION_H
#define WIDGET_BLOCK_CPP_ENUMERATION_H
#include "WidgetBlockCppBase.h"
class QCheckBox;
class QGroupBox;
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget. It provides an editable view of an enumeration C++
 * block.
 */
class Enumeration:
    public Widget::Block::Cpp::Base
{
    Q_OBJECT
    QCheckBox* _classCheckBox {nullptr};
    QGroupBox* _flagsGroupBox {nullptr};
    bool _formLayout {false};


    public:
    Enumeration(
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
     * This widget's class check box widget.
     */
    private:
    QCheckBox* classCheckBox(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's flags group box widget.
     */
    private:
    QGroupBox* flagsGroupBox(
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
