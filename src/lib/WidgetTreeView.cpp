#include "WidgetTreeView.h"
#include <QtGui>
namespace Widget {


TreeView::TreeView(
    QWidget* parent
):
    QTreeView(parent)
{
}


void TreeView::keyPressEvent(
    QKeyEvent* event
)
{
    if (
        (
            event->key() == Qt::Key_Up
            || event->key() == Qt::Key_Down
        )
        && event->modifiers() == (Qt::ControlModifier|Qt::AltModifier)
        )
    {
        event->ignore();
    }
    else
    {
        QTreeView::keyPressEvent(event);
    }
}


void TreeView::keyReleaseEvent(
    QKeyEvent* event
)
{
    if (
        (
            event->key() == Qt::Key_Up
            || event->key() == Qt::Key_Down
        )
        && event->modifiers() == (Qt::ControlModifier|Qt::AltModifier)
    )
    {
        event->ignore();
    }
    else
    {
        QTreeView::keyReleaseEvent(event);
    }
}
}
