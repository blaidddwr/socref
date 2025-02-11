#ifndef WIDGET_DIALOG_CODE_H
#define WIDGET_DIALOG_CODE_H
#include <QDialog>
#include "Global.h"
class QHBoxLayout;
class QListView;
class QStringListModel;
class QPlainTextEdit;
namespace Widget {
namespace Dialog {




/*!
 * This is a dialog widget. It displays the parsed code lines of its block
 * property.
 * 
 * block: This property is the block object whose code lines are displayed.
 */
class Code:
    public QDialog
{
    Q_OBJECT
    const AbstractBlock* _block;
    QHBoxLayout* _buttonsLayout {nullptr};
    QHBoxLayout* _viewLayout {nullptr};
    QListView* _listView {nullptr};
    QPushButton* _closeButton {nullptr};
    QPlainTextEdit* _textEdit {nullptr};
    QStringListModel* _stringListModel {nullptr};


    /*!
     * Constructor.
     *
     * @param block
     *        Initial object of the block property. This must be a valid block
     *        and cannot be destroyed during the lifetime of this object.
     */
    public:
    Code(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
        ,Qt::WindowFlags flags = Qt::WindowFlags()
    );


    /*!
     * Called when this instance's block property's destroyed signal is emitted.
     */
    private slots:
    void onBlockDestroyed(
        QObject* object
    );


    /*!
     * Called when this instance's block property's code changed signal is
     * emitted.
     */
    private slots:
    void onCodeChanged(
        const QMap<QString,QStringList>& value
    );


    /*!
     * Provides a layout for this widget.
     *
     * @return
     * The buttons layout.
     */
    private:
    QHBoxLayout* buttonsLayout(
    );


    /*!
     * Provides a child widget for this widget.
     *
     * @return
     * The close button widget.
     */
    private:
    QPushButton* closeButton(
    );


    /*!
     * Provides a child widget for this widget.
     *
     * @return
     * The list view widget.
     */
    private:
    QListView* listView(
    );


    /*!
     * Provides a child model element for this widget.
     *
     * @return
     * The string list model.
     */
    private:
    QStringListModel* stringListModel(
    );


    /*!
     * Provides a child widget for this widget.
     *
     * @return
     * The plain text edit widget.
     */
    private:
    QPlainTextEdit* textEdit(
    );


    /*!
     * Provides a layout for this widget.
     *
     * @return
     * The view layout.
     */
    private:
    QHBoxLayout* viewLayout(
    );
};
}
}


#endif
