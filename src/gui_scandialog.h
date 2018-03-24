#ifndef GUI_SCANDIALOG_H
#define GUI_SCANDIALOG_H
#include "gui_persistentdialog.h"
#include "global.h"



class QProgressBar;



namespace Gui
{
   class ScanDialog : public PersistentDialog
   {
      Q_OBJECT
   public:
      explicit ScanDialog(ScanThread* scanner, QWidget* parent = nullptr);
      virtual int exec() override final;
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
   private slots:
      void cancelClicked();
      void progressChanged(int complete);
      void scanFinished();
   private:
      void setupGui();
      void setupBar();
      QLayout* setupBottom();
      QWidget* setupCancel();
      ScanThread* _scanner;
      QProgressBar* _bar;
   };
}



#endif
