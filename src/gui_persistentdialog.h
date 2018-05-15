#ifndef GUI_PERSISTENTDIALOG_H
#define GUI_PERSISTENTDIALOG_H
#include <QDialog>
//



namespace Gui
{
   /*!
    */
   class PersistentDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit PersistentDialog(const char* geometryKey = nullptr, QWidget* parent = nullptr);
      virtual ~PersistentDialog() override;
   protected:
      void saveSettings(const char* geometryKey);
   private:
      /*!
       */
      const char* _geometryKey {nullptr};
   };
}



#endif
