#ifndef CPPQT_TYPE_EDIT_H
#define CPPQT_TYPE_EDIT_H
#include <memory>
#include <QWidget>
#include "cppqt_classes.h"



class QLineEdit;
class QTableWidget;
class QGroupBox;
class QSpinBox;



namespace CppQt
{
   namespace Type
   {
      class Edit : public QWidget
      {
         Q_OBJECT
      public:
         explicit Edit(AbstractType* type = nullptr, QWidget* parent = nullptr);
         Edit& setType(AbstractType* type);
         std::unique_ptr<AbstractType> type() const;
         Edit& setTitle(const QString& title);
         bool isValid(bool informUser = false) const;
      private slots:
         void sizeValueChanged(int i);
      private:
         void createNameEdit();
         void createSpinBox();
         void createTable();
         void createGroupBox(QLayout* layout);
         QLineEdit* _nameEdit;
         QSpinBox* _spinBox;
         QTableWidget* _variantTable;
         QGroupBox* _groupBox;
      };
   }
}



#endif
