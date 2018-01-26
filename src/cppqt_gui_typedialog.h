#ifndef CPPQT_GUI_TYPEDIALOG_H
#define CPPQT_GUI_TYPEDIALOG_H
#include <QDialog>



class QLineEdit;



namespace CppQt
{
   namespace Gui
   {
      class TypeDialog : public QDialog
      {
         Q_OBJECT
      public:
         explicit TypeDialog(QWidget* parent = nullptr);
         explicit TypeDialog(const QString& name, QWidget* parent = nullptr);
         QString name();
         void setName(const QString& name);
         static const char* _typeRegExp;
      private slots:
         void textChanged(const QString& text);
      private:
         void createGui();
         void createLineEdit();
         QLayout* createButtons();
         QPushButton* _ok;
         QLineEdit* _text;
      };
   }
}



#endif
