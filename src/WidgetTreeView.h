#ifndef WIDGET_TREEVIEW_H
#define WIDGET_TREEVIEW_H
#include <QTreeView>
namespace Widget {




/*!
 * This is an widget class. It extends the Qt tree view class.
 * 
 * This extension overrides the key press event interfaces, ignoring the
 * shortcut keys used by the project widget's move up and down actions. Any
 * other key press event is passed onto the parent Qt tree view class to act as
 * normal.
 */
class TreeView:
    public QTreeView
{
    Q_OBJECT
    public:
    using QTreeView::QTreeView;


    protected:
    virtual void keyPressEvent(
        QKeyEvent* event
    ) override final;


    protected:
    virtual void keyReleaseEvent(
        QKeyEvent* event
    ) override final;
};
}


#endif
