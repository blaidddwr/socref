#ifndef GUI_ABSTRACTEDIT_H
#define GUI_ABSTRACTEDIT_H
#include <memory>
#include <QDialog>



class QPushButton;



namespace Gui
{
   class AbstractEdit : public QDialog
   {
      Q_OBJECT
   public:
      explicit AbstractEdit(QWidget* parent);
      AbstractEdit* initialize();
   protected:
      virtual QLayout* layout() = 0;
      void setDisabled(bool disabled);
   protected:
      virtual bool apply() = 0;
   private slots:
      void okClicked();
      void applyClicked();
      void cancelClicked();
   private:
      QLayout* setupButtons();
      void setupOkButton();
      void setupApplyButton();
      QPushButton* setupCancelButton();
      QPushButton* _ok;
      QPushButton* _apply;
   };
}



#endif
