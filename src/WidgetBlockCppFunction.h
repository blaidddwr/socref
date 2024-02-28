#ifndef WIDGET_BLOCK_CPP_FUNCTION_H
#define WIDGET_BLOCK_CPP_FUNCTION_H
#include "Widget.h"
#include "WidgetBlockCppBase.h"
class QCheckBox;
class QComboBox;
class QGridLayout;
class QGroupBox;
namespace Widget {
namespace Block {
namespace Cpp {




/*!
 * This is a block widget class. It provides an editable view of a function C++
 * block.
 */
class Function:
    public Base
{
    Q_OBJECT
    QComboBox* _accessComboBox {nullptr};
    QComboBox* _assignmentComboBox {nullptr};
    QComboBox* _typeComboBox {nullptr};
    QGroupBox* _flagsGroupBox {nullptr};
    QGroupBox* _returnGroupBox {nullptr};
    QHash<int,QCheckBox*> _flagCheckBoxes;
    QLineEdit* _returnLineEdit {nullptr};
    QTextEdit* _returnDescriptionTextEdit {nullptr};
    StringList* _templateList {nullptr};
    bool _formLayout {false};
    static constexpr int FLAG_COL_SIZE {3};


    public:
    Function(
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
     * This widget's access combo box.
     */
    private:
    QComboBox* accessComboBox(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's assignment combo box.
     */
    private:
    QComboBox* assignmentComboBox(
    );


    /*!
     * Creates and returns a new combo box, initializing it with the given label
     * list, icon list, and current index.
     * 
     * The given label and icon lists must be the same size. The given current
     * index must be a valid index for the returned combo box, in other words be
     * a valid index in the given lists.
     *
     * @param labels
     *        The label list.
     *
     * @param icons
     *        The icon list.
     *
     * @param currentIndex
     *        The current index.
     */
    private:
    QComboBox* createComboBox(
        const QStringList& labels
        ,const QList<QIcon>& icons
        ,int currentIndex
    );


    /*!
     * Returns this widget's flag check box with the given function flag.
     * 
     * The given function flag must be valid.
     *
     * @param flag
     *        The function flag.
     */
    private:
    QCheckBox* flagCheckBox(
        int flag
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's flags group box.
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


    /*!
     * Getter method.
     *
     * @return
     * This widget's return description text editor.
     */
    private:
    QTextEdit* returnDescriptionTextEdit(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's return group box.
     */
    private:
    QGroupBox* returnGroupBox(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's return line editor.
     */
    private:
    QLineEdit* returnLineEdit(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's template list.
     */
    private:
    StringList* templateList(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's type combo box.
     */
    private:
    QComboBox* typeComboBox(
    );
};
}
}
}


#endif
