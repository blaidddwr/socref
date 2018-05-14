#ifndef GUI_BLOCKVIEW_H
#define GUI_BLOCKVIEW_H
#include <QSplitter>
#include <QModelIndex>
#include <QItemSelection>
#include "global.h"



class QScrollArea;
class QTreeView;
class QMenu;
class QLabel;
//



namespace Gui
{
   /*!
    * This is the primary view for an open project's block model. This class provides 
    * a tree view for the entire block model, a title, a detailed view of the 
    * currently selected block, and different actions to edit the underlying block 
    * model. Actions provided to the user through a context menu are add, remove, 
    * edit, cut, copy, paste, move up, and move down a block. These actions are 
    * updated depending on the current block selected. An external link to the context 
    * menu of this class is provided. The block model given to this class does not 
    * take ownership of it and is not responsible for its deletion. 
    */
   class BlockView : public QSplitter
   {
      Q_OBJECT
   public:
      explicit BlockView(QWidget* parent = nullptr);
      virtual ~BlockView() override final;
      QMenu* contextMenu() const;
      bool canPaste() const;
      void setModel(BlockModel* model);
   public slots:
   private slots:
      void addTriggered(int type);
      void removeTriggered();
      void editTriggered();
      void cutTriggered();
      void copyTriggered();
      void pasteTriggered();
      void moveUpTriggered();
      void moveDownTriggered();
      void selectionModelChanged(const QItemSelection& selected, const QItemSelection& deselected);
      void modelDestroyed();
      void modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
      void contextMenuRequested(const QPoint& position);
   private:
      void updateView();
      void updateIndex();
      void updateContextMenu();
      void updateActions();
      void updateAddActions();
      void updateTitle(AbstractBlock* block);
      void restoreSettings();
      void saveSettings();
      void setupGui();
      void setupTreeView();
      void setupArea();
      QLayout* setupTitleBar();
      void setupActions();
      void setupMenu();
      /*!
       * The width and height of icons, in pixels, presented in this object's tree view 
       * of blocks. 
       */
      constexpr static int _titleIconSize {32};
      /*!
       * The state key used by this object to save/restore qt settings of its state. 
       */
      static const char* _stateKey;
      /*!
       * Pointer to this object's scroll area used to contain its detailed view widget. 
       */
      QScrollArea* _area;
      /*!
       * Pointer to this object's tree view that displays the full list of its current 
       * block model. 
       */
      QTreeView* _treeView;
      /*!
       * Pointer to this object's current block model. 
       */
      BlockModel* _model {nullptr};
      /*!
       * Pointer to the abstract block factory for the project type of this object's 
       * currently set block model. 
       */
      const AbstractBlockFactory* _factory {nullptr};
      /*!
       * Pointer the selection model of this object's tree view. 
       */
      QItemSelectionModel* _selectionModel {nullptr};
      /*!
       * The currently selected model index of this object. 
       */
      QModelIndex _current;
      /*!
       * Pointer to a copied block that any instance of this class can use to paste into 
       * their current block model if possible. 
       */
      static AbstractBlock* _copy;
      /*!
       * Pointer to this object's current detailed view widget, if any. 
       */
      QWidget* _view {nullptr};
      /*!
       * Pointer list of this object's currently available add actions used for adding 
       * new blocks to the current index. 
       */
      QList<QAction*> _addActions;
      /*!
       * Pointer to this object's remove action. 
       */
      QAction* _removeAction;
      /*!
       * Pointer to this object's edit action. 
       */
      QAction* _editAction;
      /*!
       * Pointer to this object's cut action. 
       */
      QAction* _cutAction;
      /*!
       * Pointer to this object's copy action. 
       */
      QAction* _copyAction;
      /*!
       * Pointer to this object's paste action. 
       */
      QAction* _pasteAction;
      /*!
       * Pointer to this object's move up action. 
       */
      QAction* _moveUpAction;
      /*!
       * Pointer to this object's move down action. 
       */
      QAction* _moveDownAction;
      /*!
       * Pointer to this object's add menu used as a sub menu within the context menu for 
       * listing add actions. 
       */
      QMenu* _addMenu;
      /*!
       * Pointer to this object's context menu. 
       */
      QMenu* _contextMenu;
      /*!
       * Pointer to this object's title icon label used as part of the title bar above 
       * the detailed view. 
       */
      QLabel* _titleIcon;
      /*!
       * Pointer to this object's title text label used as part of the title bar above 
       * the detailed view. 
       */
      QLabel* _titleText;
   };
}



#endif
