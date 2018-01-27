#ifndef GUI_BLOCKVIEW_H
#define GUI_BLOCKVIEW_H
#include <QSplitter>
#include <QModelIndex>
#include <QItemSelection>
#include "classes.h"



class QScrollArea;
class QTreeView;
class QMenu;
class QLabel;



namespace Gui
{
   class BlockView : public QSplitter
   {
      Q_OBJECT
   public:
      explicit BlockView(QWidget *parent = nullptr);
      void setModel(BlockModel* model);
      QMenu* contextMenu() const;
      bool hasSelection() const;
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
      void editFinished();
      void titleNameChanged(AbstractBlock* block);
   private:
      void createTreeView();
      void createArea();
      QLayout* createTitleBar();
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
      constexpr static int _titleIconSize {32};
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
      QLabel* _titleIcon;
      QLabel* _titleText;
   };
}



#endif
