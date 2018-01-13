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
      explicit AbstractEdit(QWidget* parent): QWidget(parent) {}
      AbstractEdit* initialize();
   protected:
      virtual std::unique_ptr<QLayout> createForm() = 0;
   signals:
      void finished();
   protected slots:
      virtual void okClicked() = 0;
      virtual void applyClicked() = 0;
      virtual void cancelClicked() = 0;
   private:
      QLayout* createButtons();
      QPushButton* createOkButton();
      QPushButton* createApplyButton();
      QPushButton* createCancelButton();
   };
}



#endif
