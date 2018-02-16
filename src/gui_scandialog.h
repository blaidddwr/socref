#ifndef GUI_SCANDIALOG_H
#define GUI_SCANDIALOG_H
#include <QDialog>
#include "global.h"



class QProgressBar;



namespace Gui
{
   class ScanDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit ScanDialog(ScanThread* scanner, QWidget* parent = nullptr);
      virtual ~ScanDialog() override final;
      virtual int exec() override final;
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
   private slots:
      void cancelClicked();
      void progressChanged(int complete);
      void scanFinished();
   private:
      void restoreSettings();
      void saveSettings();
      void setupGui();
      void setupBar();
      QLayout* setupBottom();
      QWidget* setupCancel();
      static const char* _geometryKey;
      ScanThread* _scanner;
      QProgressBar* _bar;
   };
}



#endif
