#ifndef CPP_BLOCKWIDGET_ENUMERATIONEDIT_H
#define CPP_BLOCKWIDGET_ENUMERATIONEDIT_H
#include "CppBlockWidgetBaseEdit.h"
class QCheckBox;
class QGroupBox;
namespace Cpp {
namespace BlockWidget {




/*!
 * This is a block widget. It provides an editable view of an enumeration C++
 * block.
 */
class EnumerationEdit:
    public BaseEdit
{
    Q_OBJECT
    QCheckBox* _classCheckBox {nullptr};
    QGroupBox* _flagsGroupBox {nullptr};
    bool _formLayout {false};


    public:
    EnumerationEdit(
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


#endif
