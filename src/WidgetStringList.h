#ifndef WIDGET_STRINGLIST_H
#define WIDGET_STRINGLIST_H
#include <QListView>
class QStringListModel;
namespace Widget {




/*!
 * This is a widget class. It provides an editable view of a Qt string list.
 * 
 * It uses a Qt string list model attached to its parent Qt list view class.
 */
class StringList:
    public QListView
{
    Q_OBJECT
    QAction* _addAction {nullptr};
    QAction* _moveDownAction {nullptr};
    QAction* _moveUpAction {nullptr};
    QAction* _removeAction {nullptr};
    QMenu* _contextMenu {nullptr};
    QStringListModel* _model {nullptr};


    public:
    StringList(
        QWidget* parent = nullptr
    );


    /*!
     * Signals this widget's model's string list has changed.
     */
    signals:
    void stringListChanged(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's model.
     */
    public:
    QStringListModel* model(
    );


    /*!
     * Called when this widget's add action is triggered.
     */
    private slots:
    void add(
    );


    /*!
     * Called when this widget's move down action is triggered.
     */
    private slots:
    void moveDown(
    );


    /*!
     * Called when this widget's move up action is triggered.
     */
    private slots:
    void moveUp(
    );


    /*!
     * Called when this widget's remove action is triggered.
     */
    private slots:
    void remove(
    );


    /*!
     * Called to update this widget's actions' enabled/disabled state.
     */
    private slots:
    void updateActions(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's add action.
     */
    private:
    QAction* addAction(
    );


    /*!
     * Determines if the given index can be moved down.
     * 
     * The given index must have been created by this widget's model.
     *
     * @param index
     *        The index.
     *
     * @return
     * True if it can be moved or false otherwise.
     */
    private:
    bool canMoveDown(
        const QModelIndex& index
    );


    /*!
     * Determines if the given index can be moved up.
     * 
     * The given index must have been created by this widget's model.
     *
     * @param index
     *        The index.
     *
     * @return
     * True if it can be moved or false otherwise.
     */
    private:
    bool canMoveUp(
        const QModelIndex& index
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's context menu widget.
     */
    private:
    QMenu* contextMenu(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's move down action.
     */
    private:
    QAction* moveDownAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's move up action.
     */
    private:
    QAction* moveUpAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's remove action.
     */
    private:
    QAction* removeAction(
    );
};
}


#endif
