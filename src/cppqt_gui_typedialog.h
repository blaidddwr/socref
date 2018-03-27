#ifndef CPPQT_GUI_TYPEDIALOG_H
#define CPPQT_GUI_TYPEDIALOG_H
#include "gui_persistentdialog.h"



class QLineEdit;



namespace CppQt
{
   namespace Gui
   {
      class TypeDialog : public ::Gui::PersistentDialog
      {
         Q_OBJECT
      public:
         explicit TypeDialog(QWidget* parent = nullptr);
         explicit TypeDialog(const QString& name, QWidget* parent = nullptr);
         QString name();
         void setName(const QString& name);
         static bool isValidTypeString(const QString& input);
         static bool isValidTemplateArgument(const QString& argument);
      private slots:
         void textChanged(const QString& text);
      private:
         void setupGui();
         void setupLineEdit();
         QLayout* setupButtons();
         static const char* _geometryKey;
         QPushButton* _ok;
         QLineEdit* _text;
      };
   }
}



#endif
