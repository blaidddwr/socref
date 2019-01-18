#ifndef GUI_PERSISTENTDIALOG_H
#define GUI_PERSISTENTDIALOG_H
#include <QDialog>
//



namespace Gui
{
   /*!
    * This is a base dialog class that allows any dialog class inheriting it to keep a 
    * persistent geometry whenever the dialog is closed and opened again. This is done 
    * by using Qt settings, saving the dialog geometry upon destruction and restoring 
    * it upon construction or calling a method. A unique key must be given to this 
    * class for each dialog inheriting it to avoid Qt setting collisions of saved 
    * geometry. If a key is never given to this class then it will not save any 
    * geometry. 
    */
   class PersistentDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit PersistentDialog(QWidget* parent = nullptr);
      explicit PersistentDialog(const QString& geometryKey, QWidget* parent = nullptr);
   public slots:
      virtual void done(int r) override final;
   protected:
      virtual void closeEvent(QCloseEvent* event) override;
   protected:
      void saveSettings(const QString& geometryKey);
   private:
      /*!
       * The geometry key for this persistent dialog that must be unique for the class 
       * inheriting this one. If no key has been set this is empty. 
       */
      QString _geometryKey;
   };
}



#endif
