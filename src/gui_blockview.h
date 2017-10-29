#ifndef GUI_BLOCKVIEW_H
#define GUI_BLOCKVIEW_H
#include <QSplitter>
#include <QModelIndex>
#include <QItemSelection>



class QHBoxLayout;
class QTreeView;
class QScrollArea;
class QMenu;
class BlockModel;
class AbstractBlock;



namespace Gui {

class BlockView : public QSplitter
{
   Q_OBJECT
public:
   explicit BlockView(QWidget *parent = nullptr);
   void setModel(BlockModel* model);
   void populateAddMenu(QMenu* menu) const;
   bool hasSelection() const;
   bool canPaste() const;
signals:
   void selectionChanged();
public slots:
   void addTriggered();
   void removeTriggered();
   void editTriggered();
   void cutTriggered();
   void copyTriggered();
   void pasteTriggered();
   void moveUpTriggered();
   void moveDownTriggered();
private slots:
   void selectionModelChanged();
   void modelDestroyed();
private:
   QModelIndex getSelection() const;
   QHBoxLayout* _layout;
   QScrollArea* _area;
   QTreeView* _treeView;
   AbstractBlock* _copy {nullptr};
   QWidget* _view {nullptr};
   QList<QAction*> _addActions;
   QAction* _removeAction;
   QAction* _editAction;
   QAction* _copyAction;
   QAction* _cutAction;
   QAction* _pasteAction;
   QAction* _moveUpAction;
   QAction* _moveDownAction;
};



//@@
inline bool BlockView::hasSelection() const { return getSelection().isValid(); }

} // namespace Gui



#endif
