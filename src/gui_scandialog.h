#ifndef GUI_SCANDIALOG_H
#define GUI_SCANDIALOG_H
#include "gui_persistentdialog.h"
#include "global.h"



class QProgressBar;
//



namespace Gui
{
   /*!
    * This is the scan dialog which executes a scan thread and reports on its 
    * progress, closing once the scanning is complete or fails. Progress of scanning 
    * is presented to the user using a progress bar. This dialog allows the user to 
    * cancel the scanning process with a cancel button which requests the scan thread 
    * to interrupt itself before completion. 
    */
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
      void progressChanged(int index);
      void scanFinished();
   private:
      void setupGui();
      QLayout* setupBottom();
      /*!
       * Pointer to the scan thread object for this dialog. 
       */
      ScanThread* _scanner;
      /*!
       * Pointer to the progress bar widget for this dialog. 
       */
      QProgressBar* _bar;
   };
}



#endif
