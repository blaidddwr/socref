#ifndef WIDGET_DIALOG_CODE_H
#define WIDGET_DIALOG_CODE_H
#include <QDialog>
#include "Global.h"
#include "Model.h"
class QHBoxLayout;
class QListView;
class QPlainTextEdit;
class QSplitter;
namespace Widget {
namespace Dialog {




/*!
 * This displays the parsed code lines of a block's code property.
 * 
 * block: The block object whose code lines are displayed.
 */
class Code:
    public QDialog
{
    Q_OBJECT
    const AbstractBlock* _block;
    Model::Code* _model {nullptr};
    QHBoxLayout* _buttonsLayout {nullptr};
    QListView* _listView {nullptr};
    QPushButton* _closeButton {nullptr};
    QPlainTextEdit* _textEdit {nullptr};
    QSplitter* _splitter {nullptr};


    /*!
     * Constructor.
     *
     * @param block
     *        Initial object of the block property. This must be a valid block.
     */
    public:
    Code(
        const AbstractBlock* block
        ,QWidget* parent = nullptr
        ,Qt::WindowFlags flags = Qt::WindowFlags()
    );


    public:
    virtual void closeEvent(
        QCloseEvent* event
    ) override final;


    public:
    virtual void hideEvent(
        QHideEvent* event
    ) override final;


    /*!
     * Called when this object's block property's destroyed signal is emitted.
     */
    private slots:
    void onBlockDestroyed(
        QObject* object
    );


    /*!
     * Getter.
     *
     * @return
     * This object's buttons layout.
     */
    private:
    QHBoxLayout* buttonsLayout(
    );


    /*!
     * Getter.
     *
     * @return
     * This object's close button widget.
     */
    private:
    QPushButton* closeButton(
    );


    /*!
     * Getter.
     *
     * @return
     * This object's list view widget.
     */
    private:
    QListView* listView(
    );


    /*!
     * Getter.
     *
     * @return
     * This object's model.
     */
    private:
    Model::Code* model(
    );


    /*!
     * Restores the geometry and state of this widget using the Qt settings
     * system.
     */
    private:
    void restore(
    );


    /*!
     * Saves the geometry and state of this widget using the Qt settings system.
     */
    private:
    void save(
    ) const;


    /*!
     * Getter.
     *
     * @return
     * This object's splitter widget.
     */
    private:
    QSplitter* splitter(
    );


    /*!
     * Getter.
     *
     * @return
     * This object's plain text edit widget.
     */
    private:
    QPlainTextEdit* textEdit(
    );
};
}
}


#endif
