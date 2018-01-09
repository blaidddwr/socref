#ifndef GUI_BLOCKVIEW_H
#define GUI_BLOCKVIEW_H
#include <QSplitter>
#include <QModelIndex>
#include <QItemSelection>

#include "classes.h"



namespace Gui
{
   class BlockView : public QSplitter
   {
      Q_OBJECT
   public:
      explicit BlockView(QWidget *parent = nullptr);
      BlockView& setModel(BlockModel* model);
      QMenu* contextMenu() const { return _contextMenu; }
      bool hasSelection() const { return selection().isValid(); }
      bool canPaste() const;
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
      void editFinished() { selectionModelChanged(); }
   private:
      void createTreeView();
      void createArea();
      void createActions();
      void createRemoveAction();
      void createEditAction();
      void createCutAction();
      void createCopyAction();
      void createPasteAction();
      void createMoveUpAction();
      void createMoveDownAction();
      void createMenu();
      void updateActions();
      void updateAddActions();
      void updateMenu();
      void setView(QWidget* view);
      void setCopy(AbstractBlock* copy);
      QModelIndex selection() const;
      QScrollArea* _area;
      QTreeView* _treeView;
      BlockModel* _model {nullptr};
      const AbstractBlockFactory* _factory {nullptr};
      QItemSelectionModel* _selectionModel {nullptr};
      static AbstractBlock* _copy;
      QWidget* _view {nullptr};
      QList<QAction*> _addActions;
      QAction* _removeAction;
      QAction* _editAction;
      QAction* _cutAction;
      QAction* _copyAction;
      QAction* _pasteAction;
      QAction* _moveUpAction;
      QAction* _moveDownAction;
      QMenu* _addMenu;
      QMenu* _contextMenu;
   };
}



#endif
