#ifndef CPPQT_TYPE_GUI_EDIT_H
#define CPPQT_TYPE_GUI_EDIT_H
#include <memory>
#include <QGroupBox>
#include "cppqt_classes.h"



class QLineEdit;
class QTableWidget;
class QGroupBox;
class QSpinBox;



namespace CppQt
{
   namespace Type
   {
      namespace Gui
      {
         class Edit : public QGroupBox
         {
            Q_OBJECT
         public:
            explicit Edit(AbstractType* type = nullptr, QWidget* parent = nullptr);
            Edit& setType(AbstractType* type);
            std::unique_ptr<AbstractType> type() const;
            bool isValid(bool informUser = false) const;
         private slots:
            void sizeValueChanged(int i);
         private:
            void createNameEdit();
            void createSpinBox();
            void createTable();
            QLineEdit* _nameEdit;
            QSpinBox* _spinBox;
            QTableWidget* _variantTable;
         };
      }
   }
}



#endif
