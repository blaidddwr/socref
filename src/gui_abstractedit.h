#ifndef GUI_ABSTRACTEDIT_H
#define GUI_ABSTRACTEDIT_H
#include <memory>
#include <QWidget>



class QPushButton;



namespace Gui
{
   class AbstractEdit : public QWidget
   {
      Q_OBJECT
   public:
      explicit AbstractEdit(QWidget* parent);
      AbstractEdit* initialize();
   protected:
      virtual QLayout* layout() = 0;
   signals:
      void finished();
   protected slots:
      virtual void okClicked() = 0;
      virtual void applyClicked() = 0;
      virtual void cancelClicked() = 0;
   private:
      QLayout* setupButtons();
      QPushButton* setupOkButton();
      QPushButton* setupApplyButton();
      QPushButton* setupCancelButton();
   };
}



#endif
