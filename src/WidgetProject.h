#ifndef WIDGET_PROJECT_H
#define WIDGET_PROJECT_H
#include <QWidget>
#include "Model.h"
class QListView;
class QSplitter;
namespace Widget {




/*!
 * This is a Qt widget class. It provides an editable view of a project model.
 */
class Project:
    public QWidget
{
    Q_OBJECT
    Model::Project* _model {nullptr};
    QAction* _copyAction {nullptr};
    QAction* _cutAction {nullptr};
    QAction* _moveDownAction {nullptr};
    QAction* _moveUpAction {nullptr};
    QAction* _pasteAction {nullptr};
    QAction* _redoAction {nullptr};
    QAction* _removeAction {nullptr};
    QAction* _undoAction {nullptr};
    QListView* _listView {nullptr};
    QMenu* _addMenu {nullptr};
    QSplitter* _splitter {nullptr};
    QWidget* _blankBlockWidget {nullptr};
    int _addActionBlockIndex {-1};


    public:
    Project(
        QWidget* parent = nullptr
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's add menu.
     */
    public:
    QMenu* addMenu(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's copy action.
     */
    public:
    QAction* copyAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's cut action.
     */
    public:
    QAction* cutAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's move down action.
     */
    public:
    QAction* moveDownAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's move up action.
     */
    public:
    QAction* moveUpAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's paste action.
     */
    public:
    QAction* pasteAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's redo action.
     */
    public:
    QAction* redoAction(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's remove action.
     */
    public:
    QAction* removeAction(
    );


    /*!
     * Sets this widget's project model to the given model.
     *
     * @param model
     *        The model.
     */
    public:
    void setModel(
        Model::Project* model
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's undo action.
     */
    public:
    QAction* undoAction(
    );


    /*!
     * Called when this widget's add action is triggered with the given block
     * index.
     *
     * @param index
     *        The block index.
     */
    private slots:
    void add(
        int index
    );


    /*!
     * Called when this widget's copy action is triggered.
     */
    private slots:
    void copy(
    );


    /*!
     * Called when this widget's cut action is triggered.
     */
    private slots:
    void cut(
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
     * Called when this widget's list view's item selection model's current
     * index changed signal is emitted.
     */
    private slots:
    void onCurrentIndexChanged(
        const QModelIndex& current
        ,const QModelIndex& previous
    );


    /*!
     * Called when this widget's project model's destroyed signal is emitted.
     */
    private slots:
    void onModelDestroyed(
        QObject* object
    );


    /*!
     * Called when this widget's list view's item selection model's selection
     * changed signal is emitted.
     */
    private slots:
    void onSelectionChanged(
    );


    /*!
     * Called when this widget's paste action is triggered.
     */
    private slots:
    void paste(
    );


    /*!
     * Called when this widget's redo action is triggered.
     */
    private slots:
    void redo(
    );


    /*!
     * Called when this widget's remove action is triggered.
     */
    private slots:
    void remove(
    );


    /*!
     * Called when this widget's undo action is triggered.
     */
    private slots:
    void undo(
    );


    /*!
     * Getter method.
     */
    private:
    QWidget* blankBlockWidget(
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's list view.
     */
    private:
    QListView* listView(
    );


    /*!
     * Set this widget's block widget to the given widget.
     *
     * @param widget
     *        The widget.
     */
    private:
    void setBlockWidget(
        QWidget* widget
    );


    /*!
     * Getter method.
     *
     * @return
     * This widget's splitter.
     */
    private:
    QSplitter* splitter(
    );


    /*!
     * Updates this widget's actions.
     * 
     * The given index is this instance's list view's item selection model's
     * current index. If this instance has no model then the given index is
     * invalid.
     *
     * @param index
     *        The index.
     */
    private:
    void updateActions(
        const QModelIndex& index
    );


    /*!
     * Updates this widget's add actions, removing the list of previous add
     * actions and generating new add actions.
     * 
     * The given index is this instance's list view's item selection model's
     * current index. If this instance has no model then the given index is
     * invalid.
     *
     * @param index
     *        The index.
     */
    private:
    void updateAddActions(
        const QModelIndex& index
    );
};
}


#endif
