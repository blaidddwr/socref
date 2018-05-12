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
      void updateView();
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
      void updateActions();
      void updateAddActions();
      void updateMenu();
      void setCopy(AbstractBlock* copy);
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
       */
      constexpr static int _titleIconSize {32};
      /*!
       */
      static const char* _stateKey;
      /*!
       */
      QScrollArea* _area;
      /*!
       */
      QTreeView* _treeView;
      /*!
       */
      BlockModel* _model {nullptr};
      /*!
       */
      const AbstractBlockFactory* _factory {nullptr};
      /*!
       */
      QItemSelectionModel* _selectionModel {nullptr};
      /*!
       */
      QModelIndex _current;
      /*!
       */
      static AbstractBlock* _copy;
      /*!
       */
      QWidget* _view {nullptr};
      /*!
       */
      QList<QAction*> _addActions;
      /*!
       */
      QAction* _removeAction;
      /*!
       */
      QAction* _editAction;
      /*!
       */
      QAction* _cutAction;
      /*!
       */
      QAction* _copyAction;
      /*!
       */
      QAction* _pasteAction;
      /*!
       */
      QAction* _moveUpAction;
      /*!
       */
      QAction* _moveDownAction;
      /*!
       */
      QMenu* _addMenu;
      /*!
       */
      QMenu* _contextMenu;
      /*!
       */
      QLabel* _titleIcon;
      /*!
       */
      QLabel* _titleText;
   };
}



#endif
