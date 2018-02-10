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
      virtual void applyClicked() = 0;
   private slots:
      void okClicked();
      void cancelClicked();
   private:
      QLayout* setupButtons();
      QPushButton* setupOkButton();
      QPushButton* setupApplyButton();
      QPushButton* setupCancelButton();
   };
}



#endif
