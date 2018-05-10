#ifndef GUI_ABSTRACTEDIT_H
#define GUI_ABSTRACTEDIT_H
#include <memory>
#include "gui_persistentdialog.h"
#include "gui.h"



class QPushButton;
//



namespace Gui
{
   class AbstractEdit : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit AbstractEdit(QWidget* parent = nullptr);
      AbstractEdit* initialize(BlockView* view);
   protected:
      virtual QLayout* layout() = 0;
      void setDisabled(bool disabled);
   protected:
      virtual void apply() = 0;
   private slots:
      void okClicked();
      void applyClicked();
      void cancelClicked();
   private:
      bool tryApply();
      QLayout* setupButtons();
      void setupOkButton();
      void setupApplyButton();
      QPushButton* setupCancelButton();
      BlockView* _view;
      QPushButton* _ok;
      QPushButton* _apply;
   };
}



#endif
