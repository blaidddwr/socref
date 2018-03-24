#ifndef GUI_PERSISTENTDIALOG_H
#define GUI_PERSISTENTDIALOG_H
#include <QDialog>



class PersistentDialog : public QDialog
{
   Q_OBJECT
public:
   PersistentDialog(const char* geometryKey, QWidget* parent = nullptr);
   virtual ~PersistentDialog() override;
private:
   const char* _geometryKey;
};



#endif
